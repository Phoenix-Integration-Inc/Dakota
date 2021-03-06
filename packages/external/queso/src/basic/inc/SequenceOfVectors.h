//-----------------------------------------------------------------------bl-
//--------------------------------------------------------------------------
//
// QUESO - a library to support the Quantification of Uncertainty
// for Estimation, Simulation and Optimization
//
// Copyright (C) 2008-2017 The PECOS Development Team
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the Version 2.1 GNU Lesser General
// Public License as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc. 51 Franklin Street, Fifth Floor,
// Boston, MA  02110-1301  USA
//
//-----------------------------------------------------------------------el-

#ifndef UQ_SEQUENCE_OF_VECTORS_H
#define UQ_SEQUENCE_OF_VECTORS_H

#include <queso/VectorSequence.h>
#define UQ_SEQ_VEC_USES_SCALAR_SEQ_CODE
#undef  UQ_SEQ_VEC_USES_OPERATOR

namespace QUESO {

class GslVector;
class GslMatrix;

/*! \file SequenceOfVectors.h
 * \brief A templated class for handling vector samples
 *
 * \class SequenceOfVectors
 * \brief Class for handling vector samples (sequence of vectors).
 *
 * This class handles vector samples generated by an algorithm, as well as
 * operations that can be carried over them, e.g., calculation of means,
 * correlation and covariance matrices. It is derived from and implements
 * BaseVectorSequence<V,M>.*/

template <class V = GslVector, class M = GslMatrix>
class SequenceOfVectors : public BaseVectorSequence<V,M>
{
public:

  //! @name Class typedefs
  //@{
  typedef typename std::vector<const V*>::const_iterator seqVectorPositionConstIteratorTypedef;
  typedef typename std::vector<const V*>::iterator       seqVectorPositionIteratorTypedef;
  //@}

  //! @name Constructor/Destructor methods
  //@{
  //! Default constructor.
  SequenceOfVectors(const VectorSpace<V,M>& vectorSpace,
                           unsigned int                   subSequenceSize,
                           const std::string&             name);
  //! Destructor.
  ~SequenceOfVectors();
  //@}

  //! @name Set methods
  //@{
  //! 	Copies values from \c rhs to \c this.
  SequenceOfVectors<V,M>& operator= (const SequenceOfVectors<V,M>& rhs);
  //@}

    //! @name Sequence methods
  //@{
  //! Size of the sub-sequence of vectors.
  unsigned int subSequenceSize            () const;

  //! Resizes the sequence.
  /*! This routine deletes all stored computed vectors */
  void         resizeSequence             (unsigned int newSubSequenceSize);

  //! Resets a total of \c numPos values of the sequence starting at position  \c initialPos.
  /*! This routine deletes all stored computed vectors */
  void         resetValues                (unsigned int initialPos, unsigned int numPos);

  //! Erases \c numPos elements of the sequence starting at position  \c initialPos.
  /*! This routine deletes all stored computed vectors */
  void         erasePositions             (unsigned int initialPos, unsigned int numPos);

  //! Gets the values of the sequence at position \c posId and stores them at \c vec.
  void         getPositionValues          (unsigned int posId,       V& vec) const;

  //! Set the values in \c vec  at position \c posId  of the sequence.
  /*! This routine deletes all stored computed vectors */
  void         setPositionValues          (unsigned int posId, const V& vec);

  //! Uniformly samples from the CDF from the sub-sequence.
  void         subUniformlySampledCdf     (const V&                             numEvaluationPointsVec,
                                           ArrayOfOneDGrids <V,M>&       cdfGrids,
                                           ArrayOfOneDTables<V,M>&       cdfValues) const;

  //! Uniformly samples from the CDF from the sub-sequence.
  void         unifiedUniformlySampledCdf (const V&                             numEvaluationPointsVec,
                                           ArrayOfOneDGrids <V,M>&       unifiedCdfGrids,
                                           ArrayOfOneDTables<V,M>&       unifiedCdfValues) const;
 //! Finds the mean value of the sub-sequence, considering \c numPos positions starting at position \c initialPos.
 /*! Output: \param meanVec is the vector of the calculated means of the sub-sequence of vectors. */
  void         subMeanExtra               (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           V&                                   meanVec) const;

 //! Finds the mean value of the unified sequence, considering \c numPos positions starting at position \c initialPos.
 /*! Output: \param meanVec is the vector of the calculated means of the unified sequence of vectors. */
 void         unifiedMeanExtra           (unsigned int                         initialPos,
                                          unsigned int                         numPos,
                                          V&                                   unifiedMeanVec) const;

  //! Finds the median value of the sub-sequence, considering \c numPos positions starting at position \c initialPos.
  /*! Output: \param medianVec is the vector of the calculated medians of the sub-sequence of vectors. */
  void         subMedianExtra             (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           V&                                   medianVec) const;

  //! Finds the median value of the unfed sequence, considering \c numPos positions starting at position \c initialPos.
  /*! Output: \param medianVec is the vector of the calculated medians of the unified sequence of vectors. */
  void         unifiedMedianExtra         (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           V&                                   unifiedMedianVec) const;
  //! Finds the sample variance of the sub-sequence,  considering \c numPos positions starting at position \c initialPos and of mean \c meanVec.
  /*! Output: \param samVec is the vector of the calculated sample variance of the sub-sequence of vectors.
   * The sample variance @f$ \sigma_y^2 @f$ is the second sample central moment and is defined by
   * @f$ \sigma_y^2 = \frac{1}{n} \sum_{i=1}^n \left(y_i - \mu \right)^2 @f$, where @f$ \mu @f$ is
   * the sample mean and @f$ n @f$ is the sample size. This procedure lets the users choose the initial
   * position and the number of elements of the sequence which will be used to evaluate the sample variance.*/
  void         subSampleVarianceExtra     (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           const V&                             meanVec,
                                           V&                                   samVec) const;
  //! Finds the sample variance of the unified sequence,  considering \c numPos positions starting at position \c initialPos and of mean \c meanVec.
  /*! Output: \param samVec is the vector of the calculated sample variance of the unified sequence of vectors.*/
  void         unifiedSampleVarianceExtra (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           const V&                             unifiedMeanVec,
                                           V&                                   unifiedSamVec) const;

  //! Finds the sample standard deviation of the sub-sequence,  considering \c numPos positions starting at position \c initialPos and of mean \c meanVec.
  /*! Output: \param stdVec is the vector of the calculated sample  standard deviation of the sub-sequence of vectors.*/
  void         subSampleStd               (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           const V&                             meanVec,
                                           V&                                   stdVec) const;
  //! Finds the sample standard deviation of the unified sequence,  considering \c numPos positions starting at position \c initialPos and of mean \c meanVec.
  /*! Output: \param stdVec is the vector of the calculated sample  standard deviation of the unified sequence of vectors.*/
  void         unifiedSampleStd           (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           const V&                             unifiedMeanVec,
                                           V&                                   unifiedStdVec) const;

  //! Finds the population variance of the sub-sequence, considering \c numPos positions starting at position \c initialPos and of mean \c meanVec.
  /*! Output: \param popVec is the vector of the calculated population variance of the sub-sequence of vectors.
   * The population variance  @f$ \sigma^2 @f$ is defined by
   * @f$ \sigma^2 = \frac{1}{n-1} \sum_{i=1}^n \left(y_i - \mu \right)^2 @f$, where @f$ \mu @f$
   * is the sample mean and @f$ n @f$ is the sample size. This procedure lets the users choose the
   * initial position and the number of elements of the sequence which will be used to evaluate the
   * population variance .*/
  void         subPopulationVariance      (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           const V&                             meanVec,
                                           V&                                   popVec) const;

  //! Finds the population variance of the unified sequence, considering \c numPos positions starting at position \c initialPos and of mean \c meanVec.
  /*! Output: \param popVec is the vector of the calculated population variance of the unified sequence of vectors.*/
  void         unifiedPopulationVariance  (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           const V&                             unifiedMeanVec,
                                           V&                                   unifiedPopVec) const;

  //! Calculates the autocovariance.
  /*! The autocovariance is the covariance of a variable with itself at some other time. It is
   * calculated over a sequence of vectors with initial position \c initialPos, considering
   * \c numPos positions, a lag of \c lag, with mean given by \c meanVec. Output:
   * \param  covVec  is the vector of the calculated autocovariances of the sequence of vectors.   */
  void         autoCovariance             (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           const V&                             meanVec,
                                           unsigned int                         lag,
                                           V&                                   covVec) const;

  //! Calculates the autocorrelation via definition.
  /*! Autocorrelation is the cross-correlation of a variable with itself; it describes the
   * correlation between values of the process at different times, as a function of the two
   * times. It is calculated over a sequence of vectors with initial position \c initialPos,
   * considering \c numPos positions, a lag of \c lag, with mean given by \c meanVec. Output:
   * \param corrVec is the vector of the calculated autocorrelations of the sequence of vectors.   */
  void         autoCorrViaDef             (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           unsigned int                         lag,
                                           V&                                   corrVec) const;
  //! Calculates the autocorrelation via  Fast Fourier transforms (FFT).
  void         autoCorrViaFft             (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           const std::vector<unsigned int>&     lags,
                                           std::vector<V*>&                     corrVecs) const;

  //! Calculates the autocorrelation via  Fast Fourier transforms (FFT).
  void         autoCorrViaFft             (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           unsigned int                         numSum,
                                           V&                                   autoCorrsSumVec) const;
  //! Finds the minimum and the maximum values of the sub-sequence, considering \c numPos positions starting at position \c initialPos.
  void         subMinMaxExtra             (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           V&                                   minVec,
                                           V&                                   maxVec) const;
  //! Finds the minimum and the maximum values of the unified sequence, considering \c numPos positions starting at position \c initialPos.
  void         unifiedMinMaxExtra         (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           V&                                   unifiedMinVec,
                                            V&                                   unifiedMaxVec) const;
  //! Calculates the histogram of the sub-sequence.
  /*! It requires the specification of the number of bins that the histogram will have, the center of
   * each bin and the initial position where the data will be considered. */
  // TODO explain params minVec and maxVec
  void         subHistogram               (unsigned int                         initialPos,
                                           const V&                             minVec,
                                           const V&                             maxVec,
                                           std::vector<V*>&                     centersForAllBins,
                                           std::vector<V*>&                     quanttsForAllBins) const;
  //! Calculates the histogram of the unified sequence.
  void         unifiedHistogram           (unsigned int                         initialPos,
                                           const V&                             unifiedMinVec,
                                           const V&                             unifiedMaxVec,
                                           std::vector<V*>&                     unifiedCentersForAllBins,
                                           std::vector<V*>&                     unifiedQuanttsForAllBins) const;

  //! Returns the interquartile range of the values in the sub-sequence.
  /*! The IQR is a robust estimate of the spread of the data, since changes in the upper and
  * lower 25% of the data do not affect it. If there are outliers in the data, then the IQR
  * is more representative than the standard deviation as an estimate of the spread of the
  * body of the data. The IQR is less efficient than the standard deviation as an estimate
  * of the spread when the data is all from the normal distribution. (from Matlab)*/
  void         subInterQuantileRange      (unsigned int                         initialPos,
                                           V&                                   iqrVec) const;

  //! Returns the interquartile range of the values in the unified sequence.
  void         unifiedInterQuantileRange  (unsigned int                         initialPos,
                                           V&                                   unifiedIqrVec) const;

  //! Selects the scales (bandwidth, \c scaleVec) for the kernel density estimation, considering only the sub-sequence.
  /*! The bandwidth of the kernel is a free parameter which exhibits a strong influence on
  * the resulting estimate. Silverman (1986) suggests the following normal-based estimates:
  *   S1 = 1.06 ?? (standard deviation) ?? n^{-1/5}
  *   S2 = 0.79 ?? (\c iqrVec) ?? n^{-1/5}, where \c iqrVec is the interquartile range
  *   \c scaleVec = 0.90 ?? minimum(standard deviation, \c iqrVec /1.34) ?? n^{-1/5}.
  * These estimates are popular due to their simplicity, and are used in QUESO with the
  * adaptation of the exponent oven the sample size n (-1/5) with -1/(4 + \c kdeDimension)
  * where \c kdeDimension is the  KDE dimension.  */
  void         subScalesForKde            (unsigned int                         initialPos,
                                           const V&                             iqrVec,
                                           unsigned int                         kdeDimension,
                                           V&                                   scaleVec) const;
  //! Selects the scales (bandwidth) for the kernel density estimation, considering the unified sequence.
  void         unifiedScalesForKde        (unsigned int                         initialPos,
                                           const V&                             unifiedIqrVec,
                                           unsigned int                         kdeDimension,
                                           V&                                   unifiedScaleVec) const;
  //! Gaussian kernel for the KDE estimate of the sub-sequence.
  /*! Computes a probability density estimate of the sample in \c this sub-sequence, starting
   * at position \c initialPos. \c densityVecs is the vector of density values evaluated at
   * the points in \c evaluationParamVecs. The estimate is based on Gaussian (normal) kernel
   * function, using a window parameter (\c scaleVec).*/
  void         subGaussian1dKde           (unsigned int                         initialPos,
                                           const V&                             scaleVec,
                                           const std::vector<V*>&               evalParamVecs,
                                           std::vector<V*>&                     densityVecs) const;
  //! Gaussian kernel for the KDE estimate of the unified sequence.
  void         unifiedGaussian1dKde       (unsigned int                         initialPos,
                                           const V&                             unifiedScaleVec,
                                           const std::vector<V*>&               unifiedEvalParamVecs,
                                           std::vector<V*>&                     unifiedDensityVecs) const;
  //! Writes the sub-sequence to a file.
  /*! Given the allowed sub environments (\c allowedSubEnvIds) that are allowed to write to file,
   * together with the file name and type (\c fileName, \c fileType), it writes the entire sub-
   * sequence to the file. The sum of the initial position of the sequence (\c initialPos) with
   * the number of positions that will be written (\c numPos) must equal the size of the sequence.
   * This procedure calls void subWriteContents (unsigned int initialPos, unsigned int numPos,
   * FilePtrSetStruct& filePtrSet, const std::string& fileType). */
  void         subWriteContents           (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           const std::string&                   fileName,
                                           const std::string&                   fileType,
                                           const std::set<unsigned int>&        allowedSubEnvIds) const;
  //! Writes the sub-sequence to a file.
  /*! Uses additional variable of the type FilePtrSetStruct& to operate on files. */
  void         subWriteContents           (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           FilePtrSetStruct&                  filePtrSet,
                                           const std::string&                   fileType) const;
  //! Writes the sub-sequence to a file.
  /*! Uses object of the type std::ofstream. */
  void         subWriteContents           (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           std::ofstream&                       ofs,
                                           const std::string&                   fileType) const;
  //! Writes the unfed sequence to a file.
  //! Writes the unified sequence in Matlab/Octave format or, if enabled, in HDF5 format.
  void         unifiedWriteContents       (const std::string&                   fileName,
                                           const std::string&                   fileType) const;

  //! Reads the unified sequence from a file.
  void         unifiedReadContents        (const std::string&                   fileName,
                                           const std::string&                   fileType,
                                           const unsigned int                   subSequenceSize);
  //! TODO: It shall select positions in the sequence of vectors.
  void         select                     (const std::vector<unsigned int>&     idsOfUniquePositions);

  //! Filters positions in the sequence of vectors.
  /*! Filtered positions will starting at \c initialPos, and with spacing given by \c spacing. */
  void         filter                     (unsigned int                         initialPos,
                                           unsigned int                         spacing);

  //! Estimates convergence rate using  Brooks & Gelman method.
  double       estimateConvBrooksGelman   (unsigned int                         initialPos,
                                           unsigned int                         numPos) const;

  //! Extracts a sequence of scalars.
  /*! The sequence of scalars has size \c numPos, and it will be extracted starting at position
   * (\c initialPos, \c paramId ) of \c this sequences of vectors, given spacing \c spacing.*/
  void         extractScalarSeq           (unsigned int                         initialPos,
                                           unsigned int                         spacing,
                                           unsigned int                         numPos,
                                           unsigned int                         paramId,
                                           ScalarSequence<double>&       scalarSeq) const;

#ifdef UQ_SEQ_VEC_USES_OPERATOR
  const V*     operator[]                 (unsigned int posId) const;
  const V*&    operator[]                 (unsigned int posId);
#endif

#ifdef UQ_ALSO_COMPUTE_MDFS_WITHOUT_KDE
  void         subUniformlySampledMdf     (const V&                             numEvaluationPointsVec,
                                           ArrayOfOneDGrids <V,M>&       mdfGrids,
                                           ArrayOfOneDTables<V,M>&       mdfValues) const;
#endif

#ifdef QUESO_COMPUTES_EXTRA_POST_PROCESSING_STATISTICS
  void         subMeanCltStd              (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           const V&                             meanVec,
                                           V&                                   samVec) const;
  void         unifiedMeanCltStd          (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           const V&                             unifiedMeanVec,
                                            V&                                   unifiedSamVec) const;
  void         bmm                        (unsigned int                         initialPos,
                                           unsigned int                         batchLength,
                                           V&                                   bmmVec) const;
  void         fftForward                 (unsigned int                         initialPos,
                                           unsigned int                         fftSize,
                                           unsigned int                         paramId,
                                           std::vector<std::complex<double> >&  fftResult) const;
  //void         fftInverse                 (unsigned int fftSize);
  void         psd                        (unsigned int                         initialPos,
                                           unsigned int                         numBlocks,
                                           double                               hopSizeRatio,
                                           unsigned int                         paramId,
                                           std::vector<double>&                 psdResult) const;
  void         psdAtZero                  (unsigned int                         initialPos,
                                           unsigned int                         numBlocks,
                                           double                               hopSizeRatio,
                                           V&                                   psdVec) const;
  void         geweke                     (unsigned int                         initialPos,
                                           double                               ratioNa,
                                           double                               ratioNb,
                                                 V&                                   gewVec) const;
  void         meanStacc                  (unsigned int                         initialPos,
                                           V&                                   meanStaccVec) const;
  void         subCdfPercentageRange      (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           double                               range, // \in [0,1]
                                           V&                                   lowerVec,
                                           V&                                   upperVec) const;
  void         unifiedCdfPercentageRange  (unsigned int                         initialPos,
                                           unsigned int                         numPos,
                                           double                               range, // \in [0,1]
                                           V&                                   lowerVec,
                                           V&                                   upperVec) const;
  void         subCdfStacc                (unsigned int                         initialPos,
                                           std::vector<V*>&                     cdfStaccVecs,
                                           std::vector<V*>&                     cdfStaccVecsUp,
                                           std::vector<V*>&                     cdfStaccVecsLow,
                                           std::vector<V*>&                     sortedDataVecs) const;
  void         subCdfStacc                (unsigned int                         initialPos,
                                           const std::vector<V*>&               evalPositionsVecs,
                                           std::vector<V*>&                     cdfStaccVecs) const;
#endif
//@}
private:
  //! Copies vector sequence \c src to \c this.
  void         copy                       (const SequenceOfVectors<V,M>& src);

  //! Extracts the raw data.
  /*! This method saves in \c  rawData the data from the sequence of vectors (in private
   * attribute \c m_seq) starting at position (\c initialPos,\c paramId) , with a spacing
   * of \c spacing until \c numPos positions have been extracted. */
  void         extractRawData             (unsigned int                         initialPos,
                                           unsigned int                         spacing,
                                           unsigned int                         numPos,
                                           unsigned int                         paramId,
                                           std::vector<double>&                 rawData) const;

  //! Helper function to write matlab-specific header info for vectors
  void writeSubMatlabHeader(std::ofstream & ofs,
                            double sequenceSize,
                            double vectorSizeLocal) const;

  void writeUnifiedMatlabHeader(std::ofstream & ofs,
                                double sequenceSize,
                                double vectorSizeLocal) const;

  //! Helper function to write plain txt info for vectors
  void writeTxtHeader(std::ofstream & ofs,
                      double sequenceSize,
                      double vectorSizeLocal) const;


  using BaseVectorSequence<V,M>::m_env;
  using BaseVectorSequence<V,M>::m_vectorSpace;
  using BaseVectorSequence<V,M>::m_name;
  using BaseVectorSequence<V,M>::m_fftObj;

  //! Sequence of vectors.
  std::vector<const V*>          m_seq;

#ifdef UQ_CODE_HAS_MONITORS
  void         subMeanMonitorAlloc        (unsigned int numberOfMonitorPositions);
  void         subMeanInter0MonitorAlloc  (unsigned int numberOfMonitorPositions);
  void         unifiedMeanMonitorAlloc    (unsigned int numberOfMonitorPositions);
  void         subMeanMonitorRun          (unsigned int monitorPosition,
                                           V&           subMeanVec,
                                           V&           subMeanCltStd);
  void         subMeanInter0MonitorRun    (unsigned int currentPosition,
                                           V&           subMeanInter0Mean,
                                           V&           subMeanInter0Clt95,
                                           V&           subMeanInter0Empirical90,
                                           V&           subMeanInter0Min,
                                           V&           subMeanInter0Max);
  void         unifiedMeanMonitorRun      (unsigned int currentPosition,
                                           V&           unifiedMeanVec,
                                           V&           unifiedMeanCltStd);
  void         subMeanMonitorStore        (unsigned int i,
                                           unsigned int monitorPosition,
                                           const V&     subMeanVec,
                                           const V&     subMeanCltStd);
  void         subMeanInter0MonitorStore  (unsigned int i,
                                           unsigned int monitorPosition,
                                           const V&     subMeanInter0Mean,
                                           const V&     subMeanInter0Clt95,
                                           const V&     subMeanInter0Empirical90,
                                           const V&     subMeanInter0Min,
                                           const V&     subMeanInter0Max);
  void         unifiedMeanMonitorStore    (unsigned int i,
                                           unsigned int monitorPosition,
                                           V&           unifiedMeanVec,
                                           V&           unifiedMeanCltStd);
  void         subMeanMonitorFree         ();
  void         subMeanInter0MonitorFree   ();
  void         unifiedMeanMonitorFree     ();
  void         subMeanMonitorWrite        (std::ofstream& ofs);
  void         subMeanInter0MonitorWrite  (std::ofstream& ofs);
  void         unifiedMeanMonitorWrite    (std::ofstream& ofs);
#endif

#ifdef UQ_CODE_HAS_MONITORS
  ScalarSequence<double>* m_subMeanMonitorPosSeq;
  SequenceOfVectors<V,M>* m_subMeanVecSeq;
  SequenceOfVectors<V,M>* m_subMeanCltStdSeq;

  ScalarSequence<double>* m_subMeanInter0MonitorPosSeq;
  SequenceOfVectors<V,M>* m_subMeanInter0Mean;
  SequenceOfVectors<V,M>* m_subMeanInter0Clt95;
  SequenceOfVectors<V,M>* m_subMeanInter0Empirical90;
  SequenceOfVectors<V,M>* m_subMeanInter0Min;
  SequenceOfVectors<V,M>* m_subMeanInter0Max;

  ScalarSequence<double>* m_unifiedMeanMonitorPosSeq;
  SequenceOfVectors<V,M>* m_unifiedMeanVecSeq;
  SequenceOfVectors<V,M>* m_unifiedMeanCltStdSeq;
#endif
};

}  // End namespace QUESO

#endif // UQ_SEQUENCE_OF_VECTORS_H
