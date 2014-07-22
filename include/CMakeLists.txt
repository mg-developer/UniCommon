SET(HEADERS
    UniCommon.h
)

SET(HEADERS_UTILS
    Utils/ApplicationTimer.hpp
    Utils/CReaderImplement.h
    Utils/UniFile.h
    Utils/UniSettings.h
    Utils/UniMutex.hpp
    Utils/UniThread.hpp
    Utils/UniTimer.h
    Utils/UniException.h
)

INSTALL(FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
INSTALL(FILES ${HEADERS_UTILS}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Utils)




