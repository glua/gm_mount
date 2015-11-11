#include "GarrysMod/Lua/Interface.h"

#include "interface.h"
#include "filesystem.h"

using namespace GarrysMod;

IFileSystem *g_FileSystem;

int yourmother_Mount(lua_State *state) {
	LUA->CheckType(1, Lua::Type::NUMBER);
	LUA->CheckType(2, Lua::Type::STRING);

	FilesystemMountRetval_t mountstatus = g_FileSystem->MountSteamContent(-LUA->GetNumber(1));

	if (mountstatus == FILESYSTEM_MOUNT_OK) {
		const char *path = LUA->GetString(2);
		g_FileSystem->AddSearchPath(path, "GAME");
		g_FileSystem->AddSearchPath(path, path);
	}

	LUA->PushNumber(mountstatus);

	return 1;
}

GMOD_MODULE_OPEN() {
	CreateInterfaceFn FileSystem_StdioFactory = Sys_GetFactory("FileSystem_Stdio.dll");
	g_FileSystem = (IFileSystem *)FileSystem_StdioFactory("VFileSystem022", NULL);

	if (g_FileSystem == NULL) {
		LUA->ThrowError("gm_yourmother: Error getting IFileSystem interface.");

		return 0;
	}

	LUA->PushSpecial(Lua::SPECIAL_GLOB);
		LUA->CreateTable();
			LUA->PushCFunction(yourmother_Mount);
			LUA->SetField(-2, "Mount");
		LUA->SetField(-2, "yourmother");

		LUA->PushNumber(FILESYSTEM_MOUNT_OK);
		LUA->SetField(-2, "FILESYSTEM_MOUNT_OK");
		LUA->PushNumber(FILESYSTEM_MOUNT_FAILED);
		LUA->SetField(-2, "FILESYSTEM_MOUNT_FAILED");
	LUA->Pop();

	return 0;
}

GMOD_MODULE_CLOSE() {
	return 0;
}
