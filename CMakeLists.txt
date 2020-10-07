AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/NpcPromotion.cpp")
AC_ADD_SCRIPT_LOADER("NpcPromotion" "${CMAKE_CURRENT_LIST_DIR}/src/loader.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/mod_npc_promotion.conf.dist")
