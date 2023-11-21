-- premake5.lua
workspace "CrosswalkSim"
   configurations { "release", "debug" }

project "SIM"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++17"
   targetdir "."

   files { "**.h", "**.cpp" }

 filter "configurations:release"
      defines { "NDEBUG" }
      optimize "On"
      --flags {"LinkTimeOptimization"}

   filter "configurations:debug"
      defines { "DEBUG" }
      symbols "On"
      optimize "Debug"
