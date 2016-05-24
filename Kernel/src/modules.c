#include <modules.h>
#include <define.h>
#include <memory.h>

static void module_loadder(uint8_t ** module, void * targetModuleAddress);
static uint32_t readUInt32(uint8_t ** address);

void module_load(void * payloadStart, void ** targetModuleAddress) {
	uint32_t i;
	uint8_t * currentModule = (uint8_t*)payloadStart;
	uint32_t moduleCount = readUInt32(&currentModule);

	for(i = 0; i < moduleCount; i++) {
		module_loadder(&currentModule, targetModuleAddress[i]);
	}
}

static void module_loadder(uint8_t ** module, void * targetModuleAddress) {
	uint32_t moduleSize = readUInt32(module);
	memcpy(targetModuleAddress, *module, moduleSize);
	*module += moduleSize;
}

static uint32_t readUInt32(uint8_t ** address) {
	uint32_t result = *(uint32_t*)(*address);
	*address += sizeof(uint32_t);
	return result;
}
