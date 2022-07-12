/**
 * @file APVLinkDef.h
 * @author Rin Yokoyama (yokoyama@cns.s.u-tokyo.ac.jp)
 * @brief LinkDef for generating a ROOT dictionary
 * @version 0.1
 * @date 2022-04-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifdef __CINT__

#pragma link C++ class APVSingleData + ;
#pragma link C++ class std::vector < APVSingleData *> + ;
#pragma link C++ class APVMultiData + ;
#pragma link C++ class APVGrapeData + ;

#endif