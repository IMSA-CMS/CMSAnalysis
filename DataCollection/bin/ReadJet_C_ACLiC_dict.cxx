// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME dIuscmsdIhomesdIkdIkzhang1dIpracticedICMSSW_12_4_3dIsrcdICMSAnalysisdIDataCollectiondIbindIReadJet_C_ACLiC_dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "/uscms/homes/k/kzhang1/practice/CMSSW_12_4_3/src/CMSAnalysis/DataCollection/bin/./ReadJet.C"

// Header files passed via #pragma extra_include

namespace {
  void TriggerDictionaryInitialization_ReadJet_C_ACLiC_dict_Impl() {
    static const char* headers[] = {
"./ReadJet.C",
0
    };
    static const char* includePaths[] = {
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/lcg/root/6.24.07-bf41b0420bc269850b74e23486e2953a/include",
"/uscms/homes/k/kzhang1/practice/CMSSW_12_4_3/src",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/cms/cmssw/CMSSW_12_4_3/src",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/mctester/1.25.1-68d9249cb7865b46ffbdeb855f5a7f14/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/hydjet2/2.4.3-abe1a2e936f1c88e48cc64c68695d8d9/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/dd4hep/v01-19x-e30813dd62dd46200104b4327a5cccf1/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/herwig7/7.2.2-bf3c3693cfc854c4c136e752f2cb458f/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/tauolapp/1.1.8-c2be28b0c4449b0bbb44705ef0f1a8e9/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/sherpa/2.2.12-08f1b79a95a5091304185c55ab2255b2/include/SHERPA-MC",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/lwtnn/2.13-e05f3eef8aff045113d9ef95da20d930/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/thepeg/2.2.2-50ab34ae4be3c629ed007714a7f99a8c/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/pythia8/306-84a4765a9948f9c1a5e66f80618e2c6d/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/xrootd/5.4.2-87db138d223976139f1f5b791b25d8e4/include/xrootd/private",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/starlight/r193-9ea7cad8f726988f0b3d71894f9923c3/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/rivet/3.1.5-8ee7abd849383dd10f16e777771c6564/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/highfive/2.3.1-3587d91feefd54342d018da419c5d677/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/lhapdf/6.4.0-68defff11ffd434c73727d03802bfb85/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/geant4/10.7.2-f2f6f28d53526cbce934633504d0a84a/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/classlib/3.1.3-7a805383ac3cf7b871d22a6c66bd8f6f/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/tkonlinesw/4.2.0-1_gcc7-a301c08bcb310e9e8de6b49167884c26/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/pcre2/10.36-4c006b557b4075554eb6f6436b3189fb/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/libungif/4.1.4-9da9b254f5bd3bcb7b38d7b89774d232/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/libtiff/4.0.10-12a57e56b15d120b61b7c1f73e5f45b7/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/libpng/1.6.37-efbc28bcd11facd84b6eb25ead83f807/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/frontier_client/2.9.1-8322c58dd76df2f5fed066c5d8e40358/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/pcre/8.43-28d54724578a9006cf0dca75629374e3/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/zstd/1.4.5-9b45ad65af90ad1bcafc4228f43a8a72/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/cms/vdt/0.4.3-4cf643375686641d118ddf967ccfd199/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/valgrind/3.17.0-12ba960b086b024cce391ad228855798/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/utm/utm_0.10.0-cb8e45768738543776e4d5a303247051/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/sigcpp/3.2.0-03a2cb8734109cfe0079af5f112cd182/include/sigc++-3.0",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/sqlite/3.36.0-cdaed8943a1c7d35c738a99ec764027b/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/pacparser/1.3.7-5adb3b3fab7505e1fcf81fd8cb8e76ba/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/oracle/19.11.0.0.0dbru-e61bbe7b231454fe259beaad3a028428/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/py3-numpy/1.21.2-99508e874b8d39bfba40879990ed24fc/c-api/core/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/meschach/1.2.pCMS1-90927c923838ae1e3dfbdc370d02877d/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/lz4/1.9.2-373b1f6c80ba13e93f436c77aa63c026/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/libuuid/2.34-fe91b21b8e54a48f2d45894d9da65f14/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/libjpeg-turbo/2.0.2-4d155b847bc1dab69bb0785eff5da833/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/ktjet/1.06-b79a7631e4a781a4add6cdd817b558b5/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/jemalloc/5.3.0-83f4e92d89b06e8d4aeb7f1806365e7c/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/hls/2019.08-80454731218d87c9edf8f512cb8429f9/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/madgraph5amcatnlo/2.7.3-b017725bf7565318c7c79989eac820aa",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/heppdt/3.04.01-c965e2de7fbf97bcecf457e1ea298571/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/hector/1.3.4_patch1-01b295327c75407e16b90858b0a97642/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/gsl/2.6-84974e6daa70611c23ebff30cbb797c4/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/protobuf/3.15.1-7e10d5eb9143430224a15d6b61e33af1/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/libunwind/1.6.2-master-0de811dc1a1eae431f0b9c0fed5a85b9/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/giflib/5.2.0-7f76394971aaf24e07b4b8369893078f/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/gdbm/1.10-d603980cbd3ec2df3505d5b82740ee09/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/freetype/2.10.0-79bd022b5b599e6f3ec3b3b8bb0b7d1a/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/fftw3/3.3.8-1e8a7f04706607079fcb2980a542baed/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/fftjet/1.5.0-8aae67d0f04d0a674419252bcd1e36b6/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/fastjet/3.4.0-726cc2aa67168c4401b736ad982593f1/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/pythia6/426-604d92e9b51dd03325fbd342698d85b2/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/hepmc/2.06.10-46867a6dcc6e5712b7953fe57085fcbd/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/expat/2.1.0-e2e74851cebffc0aa0988d120ae74fcd/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/vecgeom/v1.1.17-e658aebf667adad0ef50a21a40815b01/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/vecgeom/v1.1.17-e658aebf667adad0ef50a21a40815b01/include/VecGeom",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/xerces-c/3.1.3-5b1e4ab8f5a2057565f3be0ae6a10b37/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/xz/5.2.5-d6fed2038c4e8d6e04531d1adba59f37/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/lcg/root/6.24.07-bf41b0420bc269850b74e23486e2953a/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/dcap/2.47.12-a77c8542c3e7a4c5572e17be1288579a/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/libxml2/2.9.10-32fb6ccc7a5c1482269c150e9287ec24/include/libxml2",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/curl/7.79.0-497e9b528f6eb6d9394cc8387a9475d6/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/cppunit/1.15.x-c8077257a33f4d9e49c08bb58ba5c3b1/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/cms/coral/CORAL_2_3_21-44054a566a749b209a30d6d5d5999a62/include/LCG",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/clhep/2.4.5.1-5bba156cb451d19d7be7ff651e7c5f9f/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/zlib/1.2.11-71514f01d2850dfd0bcd391557259a58/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/bz2lib/1.0.6-86270889250d9cb8193d1963706a39cb/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/tbb/v2021.5.0-e966a5acb1e4d5fd7605074bafbb079c/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/cuda/11.5.2-c927b7e765e06433950d8a7eab9eddb4/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/boost/1.78.0-22078d9ad44a9ee315c3d0995a7d3746/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/xgboost/1.3.3-6b1fe94a5d757a4af82f4de80cd9027f/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/tinyxml2/6.2.0-b4ef6e4242045ed0c243ef457f5b32a5/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/rocm/5.0.2-95c215630c939706b0552e3eee38861c/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/rocm/5.0.2-95c215630c939706b0552e3eee38861c/hip/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/rocm/5.0.2-95c215630c939706b0552e3eee38861c/hsa/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/rdma-core/39.1-9ec71a454e4029a35fd314285e6e3103/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/ittnotify/16.06.18-9dacc777a1a80aa1b3e442956e72825f/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/gosamcontrib/2.0-20150803-5884b0587d951b6d6e6d85fbb84346dd/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/fmt/8.0.1-a90b32563595b499263c17a8753d8f1d/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/external/eigen/82dd3710dac619448f50331c1d6a35da673f764a-01f0fbbc69fe5c11dd0acfe3c748f30b/include/eigen3",
"/usr/local/include",
"/usr/include",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/lcg/root/6.24.07-bf41b0420bc269850b74e23486e2953a/etc/",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/lcg/root/6.24.07-bf41b0420bc269850b74e23486e2953a/etc//cling",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/lcg/root/6.24.07-bf41b0420bc269850b74e23486e2953a/include/",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc10/lcg/root/6.24.07-bf41b0420bc269850b74e23486e2953a/include/",
"/uscms/homes/k/kzhang1/practice/CMSSW_12_4_3/src/CMSAnalysis/DataCollection/bin/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "ReadJet_C_ACLiC_dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "ReadJet_C_ACLiC_dict dictionary payload"

#ifndef __ACLIC__
  #define __ACLIC__ 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "./ReadJet.C"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"ReadJet", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("ReadJet_C_ACLiC_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_ReadJet_C_ACLiC_dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_ReadJet_C_ACLiC_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_ReadJet_C_ACLiC_dict() {
  TriggerDictionaryInitialization_ReadJet_C_ACLiC_dict_Impl();
}
