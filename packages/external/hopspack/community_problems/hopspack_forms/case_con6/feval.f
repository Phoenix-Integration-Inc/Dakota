
      SUBROUTINE feval(DIM,W,welfile,mfnfile,hedfile,FV,FLAG)
      IMPLICIT NONE
C      include "mpif.h"   (commented out by GG 08/04/03)

C************************************************************************* 
!     SUBROUTINE TO EVALUATE THE OBJECTIVE FUNCTION FOR OPTIMIZATION 
!
!     THIS OBJECTIVE FUNCTION WILL:
!       1. SET THE VALUES FOR THE SIMULATION
!       2. CALL FLOW AND TRANSPORT SIMULATORS
!       3. EXCTRACT NECESSARY INFORMATION FROM OUTPUT FILES 
!          BY CALLING THE SUBROUTINES WRITEWEL AND READHEAD
!       4. COMPUTE THE COST BY CALLING THE SUBROUTINE COSTA(C,O)
C************************************************************************
!     Program created by Katie Kavanagh 9/11/02
!     Last modified: 8/04/03 by Genetha Gray
C***********************************************************************
!     INPUT:
!             DIM: DIMENSION OF THE PROBLEM
!               V: POINT TO EVALUATE THE FUNCTION AT 
!         welfile: name of the .wel file (added by GG 08/04/03)
!         mfnfile: name of the .mfn file (added by GG 08/04/03) 
!         hedfile: name of the .hed file (added by GG 08/04/03)
!
!     OUTPUT:
!             FV: FUNCTION VALUE
!           FLAG: 0 FOR SUCCESS, >0 FOR FAILURE 
C************************************************************************
C Description of variables:
!
! NROW, NLAY, NCOL: number of rows, layers, columns    
! WELRATE: array of pumping rates
! COSTC, COSTO capital and operation costs
! TOTALCOST = COSTC + COSTO
! HEAD array containing values of head, output from modflow
C**********************************************************************
      CHARACTER*80 modflw96
C*** next line added by GG 08/04/03
      CHARACTER*80 mfnfile,welfile,hedfile 
    
      INTEGER DIM,I,J,K,FLAG,istat, mype,npes
      INTEGER NROW,NLAY,NCOL
      integer modflag
C!!!! NROW, NCOL, NLAY must match the values in otpmain.f
      PARAMETER(NROW = 50)
      PARAMETER(NCOL = 50)
      PARAMETER(NLAY = 10)

      REAL QTMIN, QT
      DOUBLE PRECISION MINRATE
      DOUBLE PRECISION V(DIM),FV,WELRATE(DIM), COSTC,COSTO,TOTALCOST
      DOUBLE PRECISION W(100)
      REAL*8 HEAD(NROW,NCOL,NLAY), DRWDOWN(DIM), STARTHEAD(DIM)
      INTEGER X(DIM), Y(DIM), Z(DIM)
      common modflag
C***********************************************************
C  Modification by GG 08/04/03 to get stand alone executable
C    Comment out MPI rank calls
C
C      call MPI_COMM_rank(MPI_COMM_WORLD,mype,istat);
C***********************************************************

C Set iterate V  (modified by GG 08/04/03)
      DO I = 1,DIM
        V(I) = W(I)
      ENDDO

C Set values (MINRATE is the shut off pumping rate value)
      MINRATE = 0.0001
      QTMIN = -0.031998
      QT = 0.0
      COSTO = 0.0
      COSTC = 0.0
C~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~            
C Set your well locations
       DO 90 , I=1,DIM/3
         Z(I) = 10
 90      CONTINUE


      
       X(1) = NINT((1000-V(2))/20.0) 
       Y(1) = NINT(V(1)/20.0)
  
       X(2) = NINT((1000-V(4))/20.0) 
       Y(2) = NINT(V(3)/20.0)

       X(3) = NINT((1000-V(6))/20.0) 
       Y(3) = NINT(V(5)/20.0)
  
       X(4) = NINT((1000-V(8))/20.0) 
       Y(4) = NINT(V(7)/20.0)  

       X(5) = NINT((1000-V(10))/20.0) 
       Y(5) = NINT(V(9)/20.0)

       X(6) = NINT((1000-V(12))/20.0) 
       Y(6) = NINT(V(11)/20.0) 
  
C~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
C Set the pumping rates at the 6 wells     
       DO 666 , I=1,DIM/3
       WELRATE(I) = V(I+12) 
       WRITE(*,*) WELRATE(I), MINRATE
       IF(ABS(WELRATE(I)) .LE. MINRATE)THEN
       WELRATE(I) = 0.0
       WRITE(*,*) I, WELRATE(I)
       ENDIF
 666   CONTINUE
C~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  
C Set FLAG = 0 for success
      FLAG = 0
C~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
C Check that 2 wells do not occupy the same location
      call location_constraint(DIM/3,X,Y,FLAG)

C~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
C Check the constraints on the net pumping rate
      call linear_constraint(DIM/3,X,Y,Z,WELRATE,NLAY,
     & NROW, NCOL,FLAG)
C~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      IF(FLAG == 1)THEN
      modflag = 1
      ENDIF

C Compute the cost:      
      IF(FLAG .EQ. 0)THEN

C***********************************************************
C  Modification by GG 08/04/03 to get stand alone executable
C    mfn file will be generated by C script that calls stand alone
C    executable; mype no longer used
C
C Set the .mfn file for modflow
!      call writemfn(mype)
!      if(mype<10)then
!      write(mfnfile,'(a2,i1,a4)') 'A1',mype,'.mfn'
!      endif
!      if(mype>9)then
!      write(mfnfile,'(a2,i2,a4)') 'A1',mype,'.mfn'
!      endif
C***********************************************************

C Change the .wel file before calling MODFLOW
C***********************************************************
C  Modification by GG 08/04/03 to get stand alone executable
C    name of the .wel file will be predetermined and passed into the
C    the write wel subroutine instead of passing in mype
C
!      call writewel(DIM/2,X,Y,Z,WELRATE,mype)
C***********************************************************
      call writewel(DIM/3,X,Y,Z,WELRATE,welfile)
      CLOSE(13)

C Call MODFLOW
      call system("echo "//mfnfile//" | modflw96")

C Read in the heads from the .hed file that MODFLOW outputs
C***********************************************************
C  Modification by GG 08/04/03 to get stand alone executable
C    name of .hed file passed in instead of using mype
C
!      call readhead(NROW,NCOL,NLAY,HEAD,mype)
C***********************************************************
      call readhead(NROW,NCOL,NLAY,HEAD,hedfile)
C      DO 10 , K=1,NLAY
C      WRITE(*,*)  ((HEAD(I,J,K),J=1,NCOL),I=1,NROW)
C 10   CONTINUE


C Compute the installation cost
      call costac(DIM/3,X,Y,Z,WELRATE,NLAY,NROW,NCOL,COSTC,FLAG)
C      WRITE(*,*) 'costc', COSTC

C Compute the operational cost
      call costao(DIM/3,X,Y,Z,WELRATE,HEAD,NLAY,NROW,NCOL,COSTO,FLAG)
C      WRITE(*,*) 'costo', COSTO

      FV = COSTC + COSTO

C Check to see if the constraints are violated
      call head_constraint(DIM/3,X,Y,Z,WELRATE,HEAD,NLAY,NROW,
     & NCOL,COSTO,FLAG)

C***********************************************************
C  Modification by GG 08/04/03 to get stand alone executable
C    write the function value and flag to a file in EvalFn.f
C    instead of writing it to the screen here
C
C      WRITE(*,*) '********************************'
C      WRITE(*,100) ' Function value =', FV
C      WRITE(*,*) 'FLAG=', FLAG
C      WRITE(*,*) '********************************'
C100   FORMAT(a17,E13.5)
C***********************************************************

      ENDIF

      END


