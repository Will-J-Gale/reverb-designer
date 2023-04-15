/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   APF_xml;
    const int            APF_xmlSize = 3673;

    extern const char*   FeedbackComb_xml;
    const int            FeedbackComb_xmlSize = 3092;

    extern const char*   FeedforwardComb_xml;
    const int            FeedforwardComb_xmlSize = 3137;

    extern const char*   Figure8Reverb_xml;
    const int            Figure8Reverb_xmlSize = 13386;

    extern const char*   SchroederReverb_xml;
    const int            SchroederReverb_xmlSize = 6852;

    extern const char*   SpinSemiconductorReverb_xml;
    const int            SpinSemiconductorReverb_xmlSize = 13192;

    extern const char*   StereoDelay_xml;
    const int            StereoDelay_xmlSize = 3896;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 7;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
