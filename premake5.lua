solution "gm_yourmother"
	language		"C++"
	architecture	"x86"
	location		"project"
	targetdir		"bin"

	--
	-- Statically link the C-Runtime to reduce dependencies needed to run our module
	--
	flags "StaticRuntime"

	configurations { "Release" }

	configuration "Release"
		flags		"symbols"	-- Generate debugging information
		optimize	"On"		-- Optimize the build output for size and speed

	project "gm_yourmother"
		kind	"SharedLib"
		include	"LuaInterface"
		include	"SourceSDK/Tier0"
		include	"SourceSDK/Tier1"

		files { "src/**.cpp", "src/**.h" }
