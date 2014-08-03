#ifndef PLATFORMER_COMMON_H
#define PLATFORMER_COMMON_H

#include <array>
#include "gameplay.h"

namespace platformer
{
    #define PLATFORMER_ASSERT(expression, ...) if (!(expression)) GP_ERROR(__VA_ARGS__)
    #define PLATFORMER_ASSERTFAIL(...) PLATFORMER_ASSERT(false, __VA_ARGS__)

    #define PLATFORMER_LOG(message, ...)                                                                                                            \
    {                                                                                                                                               \
        char logTimeStamp[UCHAR_MAX];                                                                                                               \
        sprintf(logTimeStamp, "[%.2f] ", gameplay::Game::getAbsoluteTime() / 1000.0f);                                                              \
        gameplay::Logger::log(gameplay::Logger::Level::LEVEL_INFO, (std::string(logTimeStamp) + std::string(message) + "\n").c_str(), __VA_ARGS__); \
    }

    #define PLATFORMER_RANDOM_RANGE(min, max) min + (fabs(min - max) * MATH_RANDOM_0_1())
    #define PLATFORMER_FORCE_RELEASE(ref) forceReleaseRef(ref)
    #define PLATFORMER_ASSERT_SINGLE_REF(ref) PLATFORMER_ASSERT(ref->getRefCount() == 1, "Ref has references still outstanding")
    #define PLATFORMER_SAFE_DELETE_AI_MESSAGE(message) gameplay::AIMessage::destroy(message); message = nullptr
    #define PLATFORMER_FONT_SIZE_SMALL 20
    #define PLATFORMER_FONT_SIZE_REGULAR 35
    #define PLATFORMER_FONT_SIZE_LARGE 50
    #define PLATFORMER_FONT_SIZE_REGULAR_INDEX 0
    #define PLATFORMER_FONT_SIZE_LARGE_INDEX 1
    #define PLATFORMER_MAX_GAMEPADS 1
    #define PLATFORMER_UNIT_SCALAR 0.01f

    /**
     * Calls gameplay::Properties::create() and also logs the file url.
     *
     * Gameplay doesn't provide the file name when loading a properties files
     * fails due to invalid formatting.
     *
     * @script{ignore}
    */
    gameplay::Properties * createProperties(const char * url);

    /** @script{ignore} */
    void loggingCallback(gameplay::Logger::Level level, char const * msg);

    /**
     * Recursivley calls release() on the ref until it has been deleted
     *
     * @script{ignore}
    */
    void forceReleaseRef(gameplay::Ref * ref);

    /**
     * Defines an interface which serves to categorise data that is attached to a node via
     * gameplay::Node::setUserPointer. This allows multiple systems to make safe use of node
     * data caching at the same time.
     *
     * @script{ignore}
    */
    class INodeUserData
    {
    public:
        virtual int getNodeUserDataId() const = 0;
    };

    template<typename NodeUserDataType>
    /** @script{ignore} */
    NodeUserDataType * getNodeUserData(gameplay::Node const * node, int id);

#ifndef _FINAL
    #define PLATFORMER_ON_SCREEN_LOG_HISTORY_CAPACITY 25

    /** @script{ignore} */
    void clearLogHistory();
#endif
}

#include "Common.inl"

#endif
