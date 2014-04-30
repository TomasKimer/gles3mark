NDK_TOOLCHAIN_VERSION := 4.8
#NDK_TOOLCHAIN_VERSION := clang
APP_STL := gnustl_shared  #gnustl_static #stlport_static
# APP_OPTIM := debug
# ndk-build NDK_DEBUG=0

# Enable c++11 extentions in source code
# APP_CPPFLAGS += -std=c++11