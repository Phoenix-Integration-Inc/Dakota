C***********************************************************************
C LHS (Latin Hypercube Sampling) UNIX Library/Standalone. 
C Copyright (c) 2004, Sandia Corporation.  Under the terms of Contract
C DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government
C retains certain rights in this software.
C
C This software is distributed under the GNU Lesser General Public License.
C For more information, see the README file in the LHS directory. 
C***********************************************************************
C     Last change:  SLD  29 Mar 101    9:00 am
C****************************************************************
C SUBROUTINE BETALN IS USED IN GENERATING A BETA DISTRIBUTION
C
      DOUBLE PRECISION FUNCTION BETALI(A,B,gamln)
cc    only 2001 sld changes were comments                               sld01
cc    BETALN is called from BETAIC                                      sld01
cc    BETALN has entry point GAMALN                                     sld01
C     INCLUDE 'KILLFILE.INC'                                            GDW-96
      USE KILLFILE
C
C     WRITTEN BY D.E. AMOS AND S.L. DANIEL, MARCH, 1975.
C
C     REFERENCE SC-DR-69 591 AND SAND-75-0152
C
C     ABSTRACT
C         FUNCTION BETALN COMPUTES THE NATURAL LOG OF THE COMPLETE BETA
C         FUNCTION, LN BETA(A,B), WHERE BETA(A,B) IS DEFINED IN TERMS OF
C         THE GAMMA FUNCTION BY BETA(A,B)=GAMMA(A)*GAMMA(B)/GAMMA(A+B).
C         THE NATURAL LOG OF THE GAMMA FUNCTION IS COMPUTED WITH THE
C         LOGARITHMIC TERMS COMBINED ANALYTICALLY TO MINIMIZE LOGARITHM
C         EVALUATIONS. A RATIONAL CHEBYSHEV APPROXIMATION ON (8,1000)
C         AND THE ASYMPTOTIC EXPANSION FOR X.GT.1000 COMPLETES THE
C         CALCULATION OF THE NON-LOGARITHMIC TERMS. BACKWARD RECURSION
C         ON GAMMA(X-1)=GAMMA(X)/(X-1) AND THE LOGARITHM OF THE RESULT
C         SUFFICES FOR X.LT.8.
C
C     DESCRIPTION OF ARGUMENTS
C
C         INPUT
C           A      - ARGUMENT, A.GT.0.
C           B      - ARGUMENT, B.GT.0.
C         OUTPUT
C           BETALN - VALUE FOR LN BETA(A,B)
C
C     ERROR CONDITIONS
C         INPUT PARAMETER NON-POSITIVE  - A FATAL ERROR
C
C
C
C     BETALN USES SUBROUTINES ERRCHK, ERRGET, ERRPRT, ERXSET, ERSTGT
cc    ERRPRT no longer used                                             sld01
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
	logical gamln
      DIMENSION G(8),P(5),Q(2)
C
      DATA XLIM1,XLIM2,RTWPIL,PI/ 8.  ,  1000.   , 9.18938533204673E-01,
     1 3.14159265358979E+00/
C
      DATA  G              / 8.52516136106541E+00, 6.57925121201010E+00,
     1 4.78749174278205E+00, 3.17805383034795E+00, 1.79175946922806E+00,
     2 6.93147180559945E-01,         2*0.0       /
C
      DATA  P              / 7.66345188000000E-04,-5.94095610520000E-04,
     1 7.93643110484500E-04,-2.77777775657725E-03, 8.33333333333169E-02/
C
      DATA  Q              /-2.77777777777778E-03, 8.33333333333333E-02/
C
C
      IF(A.LE.0.) GO TO 91
	if (gamln) goto 100
*	BETAIC calls GAMALN with B == 0, but A should be positive
      IF(B.LE.0.) GO TO 91
      APB=A+B
      IF(APB.LE.8.) GO TO 20
      IF(A.LE.8.) GO TO 15
      IF(B.GT.8.) GO TO 40
      GO TO 60
   15 IF(B.GT.8.) GO TO 65
C
   20 IFLAG=0
      ASSIGN 25 TO ISET
      X=APB
      GO TO 110
   25 T1=GAML
      X=A
      ASSIGN 30 TO ISET
      GO TO 110
   30 T2=GAML
      X=B
      ASSIGN 35 TO ISET
      GO TO 110
   35 RLBETA=T1-T2-GAML
      BETALI=-RLBETA
      RETURN
C
   40 IFLAG=1
      ASSIGN 45 TO ISET
      X=APB
      GO TO 110
   45 T1=GAML
      ASSIGN 50 TO ISET
      X=A
      GO TO 110
   50 T2=GAML
      ASSIGN 55 TO ISET
      X=B
      GO TO 110
   55 SS=T1-T2-GAML
      RLBETA=A*LOG(1.+B/A)+B*LOG(1.+A/B)+
     1 .5*LOG(A*B/APB)+SS-RTWPIL
      BETALI=-RLBETA
      RETURN
C
   60 D2=A
      D3=B
      GO TO 70
   65 D2=B
      D3=A
   70 D1=APB
      IFLAG=1
      X=D1
      ASSIGN 75 TO ISET
      GO TO 110
   75 T1=GAML
      ASSIGN 80 TO ISET
      X=D2
      GO TO 110
   80 T2=GAML
      SS=T1-T2
      IFLAG=0
      X=D3
      ASSIGN 85 TO ISET
      GO TO 110
   85 RLBETA=(D2-.5)*LOG(1.+D3/D2)+D3*LOG(D1)-D3-GAML+SS
      BETALI=-RLBETA
      RETURN
C
C     IFLAG=0 FOR X.LE.8 AND LN GAMMA(X) IS RETURNED AT 300
C     IFLAG=1 FOR X.GT.8 AND LN GAMMA(X) LESS LOG FACTORS IS RETURNED
C     AT 300
C
*     ENTRY GAMALN
  100 IFLAG=0
      ASSIGN 160 TO ISET
      X=A
      IF(X) 90,90,110
  110 DX=X-XLIM1
      IF(DX) 120,150,140
  120 IF(X-1.) 121,152,123
  121 XZ=X+8.
      IX=INT(X)
      FXZ=X-IX
      TX=X
      FK=-.5
      NDX=7
      GO TO 126
  123 DX=ABS(DX)
      NDX=DX
      DNDX=NDX
      NDX=NDX+1
      IF((DNDX-DX).EQ.0.) GO TO 151
      XZ=X+DNDX+1.
      IX=INT(X)
      FXZ=X-IX
      TX=1.
      FK=.5
  126 CONTINUE
      IXZ=INT(XZ)
      XZI=FLOAT(IXZ)
      RX=1.0/XZ
      RXX=RX*RX
      DO 124 I=1,NDX
      XZI=XZI-1.0
      XK=(XZI+FXZ)/XZ
      TX=TX*XK
  124 CONTINUE
      SUM=(X-FK)*LOG(XZ)-LOG(TX)-XZ
      PX=P(1)
      DO 125 I=2,5
      PX=PX*RXX+P(I)
  125 CONTINUE
      GAML=PX*RX+SUM+RTWPIL
      GO TO 300
  140 RX=1./X
      RXX=RX*RX
      IF((X-XLIM2).LT.0.) GO TO 141
      PX=Q(1)*RXX+Q(2)
      IF(IFLAG.EQ.1) GO TO 200
      GAML=PX*RX+(X-.5)*LOG(X)-X+RTWPIL
      GO TO 300
  141 PX=P(1)
      SUM=(X-.5)*LOG(X)-X
      DO 142 I=2,5
      PX=PX*RXX+P(I)
  142 CONTINUE
      IF(IFLAG.EQ.1) GO TO 200
      GAML=PX*RX+SUM+RTWPIL
      GO TO 300
  150 GAML=G(1)
      GO TO 300
  151 GAML=G(NDX)
      GO TO 300
  152 GAML=G(8)
      GO TO 300
  160 BETALI=GAML
      RETURN
  200 GAML=PX*RX
  300 GO TO ISET,(25,30,35,45,50,55,75,80,85,160)
C
   90 CALL ERRCHK(61,  'IN BETALN, ARGUMENT FOR GAMALN IS LESS THAN OR E
     1QUAL TO ZERO.')
      If(KLLERR) Return
      RETURN
   91 CALL ERRCHK(53,  'IN BETALN, AN ARGUMENT IS LESS THAN OR EQUAL TO
     1ZERO.')
      If(KLLERR) Return
      RETURN
      END
	double precision function betaln(a,b)
	double precision a, b, betali
	external betali
	betaln = betali(a, b, .false.)
	end
	double precision function gamaln(a,b)
	double precision a, b, betali
	external betali
	gamaln = betali(a, b, .true.)
	end
