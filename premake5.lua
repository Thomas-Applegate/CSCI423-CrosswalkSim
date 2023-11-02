-- premake5.lua
workspace "CrosswalkSim"
   configurations { "debug", "release" }

project "SIM"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "bin/%{cfg.buildcfg}"

   files { "**.h", "**.cpp" }

 filter "configurations:release"
      defines { "NDEBUG" }
      optimize "On"

   filter "configurations:debug"
      defines { "DEBUG" }
      symbols "On"
      optimize "Debug"
