#include <cmath>
#include <fstream>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <sys/time.h>
#include <thread>

#include "ConstraintFile.H"
#include "Coverage.h"
// #include "OptionTupleSet.h"
#include "LineVarTupleSet.h"
#include "SAT.H"
#include "Tabu.h"
#include "TestSetFile.H"
#include "ThreadTmpResult.h"
#include "TupleSet.h"
#include "Valid_check.h"
#include "mersenne.h"

class CoveringArray {
public:
  CoveringArray(const SpecificationFile &specificationFile,
                const ConstraintFile &constraintFile, TestSetFile &testSet,
                unsigned long long maxT, int seed, int threadsNum);
  ~CoveringArray();
  void greedyConstraintInitialize();
  void greedyConstraintInitialize2();
  void actsInitialize(const std::string file_name);
  void optimize();

private:
  Valid::Validater validater;
  SATSolver satSolver;
  std::vector<bool> option_constrained_indicator;
  Mersenne mersenne;
  const SpecificationFile &specificationFile;
  TestSetFile &testSet;
  std::vector<std::vector<unsigned>> bestArray; // = array;
  std::vector<std::vector<unsigned>> array;
  Coverage coverage;
  TupleSet uncoveredTuples;
  std::set<unsigned> varInUncovertuples;
  LineVarTupleSet oneCoveredTuples;
  Tabu<Entry> entryTabu;

  unsigned long long maxTime;
  unsigned long long step;
  struct timeval start_time;

  int minTaskSize = 100;
  std::vector<std::atomic<bool> *> taskReadyPtr;
  std::vector<std::thread *> threadsPtr;
  std::vector<std::packaged_task<void()>> tasks;
  std::atomic<bool> programStop;
  int threadsNum;

  std::mutex uncoveredTuplesMutex;
  std::mutex oneCoveredTuplesMutex;
  std::vector<unsigned> columns1;
  std::vector<unsigned> columns2;

  void cover(const unsigned encode, const unsigned oldLineIndex);
  void cover_with_lock(const unsigned encode, const unsigned oldLineIndex);
  void uncover(const unsigned encode, const unsigned oldLineIndex);
  void uncover_with_lock(const unsigned encode, const unsigned oldLineIndex);
  void updateTestSet();
  // produce one row at least cover one uncovered tuple.
  // Producing the row without update coverage
  void produceSatRow(std::vector<unsigned> &newLine, const unsigned encode);
  // greedily produce one row at least cover one uncovered tuple.
  // producing the row AND updating coverage
  void mostGreedySatRow(std::vector<unsigned> &newLine, const unsigned encode);
  void mostGreedySatRow2(std::vector<unsigned> &newLine, const unsigned encode);
  void replaceRow(const unsigned lineIndex, const unsigned encode);
  void replaceRow2(const unsigned lineIndex, const unsigned encode);
  void removeUselessRows();
  void removeUselessRows2();
  void removeOneRow();
  void removeOneRowGreedy();
  void removeOneRowRandom();
  long long varScoreOfRow(const unsigned var, const unsigned lineIndex);
  long long varScoreOfRow2(const unsigned var, const unsigned lineIndex);
  long long varScoreOfRow3(const unsigned var, const unsigned lineIndex);
  void replace(const unsigned var, const unsigned lineIndex);

  long long multiVarRow(const std::vector<unsigned> &sortedMultiVars,
                        const unsigned lineIndex, const bool change = false);
  long long multiVarScoreOfRow(const std::vector<unsigned> &sortedMultiVars,
                               const unsigned lineIndex);
  long long multiVarScoreOfRow2(const std::vector<unsigned> &sortedMultiVars,
                                const unsigned lineIndex);
  void multiVarReplace(const std::vector<unsigned> &sortedMultiVars,
                       const unsigned lineIndex);

  void tabuStep();
  void tabuStep2();
  void tabuStep3();
  void tabuStep4();
  void tabuZero();
  void tabugw();
  void tabugwParallel();
  void tabugwSubTask(const size_t start_index, const size_t end_index,
                     const unsigned &base, ThreadTmpResult &threadTmpResult);
  void tmpPrint();
  bool verify(const std::vector<std::vector<unsigned>> &resultArray);
  bool checkCovered(unsigned encode);

#ifndef NDEBUG
  void print();
#endif
  void t();
};
