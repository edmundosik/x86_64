#include "pci.h"

namespace PCI {
    void EnumerateFunction(uint64_t deviceAddress, uint64_t function) {
        uint64_t offset = function << 12;
        uint64_t functionAddress = deviceAddress + offset;
        gPageTableManager.MapMemory((void*)functionAddress, (void*)functionAddress);

        PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)functionAddress;

        if(pciDeviceHeader->DeviceID == 0)
            return;
        if(pciDeviceHeader->DeviceID == 0xFFFF)
            return;

        renderer->print(GetVendorName(pciDeviceHeader->VendorID));
        renderer->print(" / ");
        renderer->print(GetDeviceName(pciDeviceHeader->VendorID, pciDeviceHeader->DeviceID));
        renderer->print(" / ");
        renderer->print(DeviceClasses[pciDeviceHeader->Class]);
        renderer->print(" / ");
        renderer->print(GetSubclassName(pciDeviceHeader->Class, pciDeviceHeader->Subclass));
        renderer->print(" / ");
        renderer->print(GetProgIFName(pciDeviceHeader->Class, pciDeviceHeader->Subclass, pciDeviceHeader->ProgIF));
        renderer->nextLine();
    }

    void EnumerateDevice(uint64_t baseAddress, uint64_t device) {
        uint64_t offset = device << 15;
        uint64_t deviceAddress = baseAddress + offset;
        gPageTableManager.MapMemory((void*)deviceAddress, (void*)deviceAddress);

        PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)deviceAddress;

        if(pciDeviceHeader->DeviceID == 0)
            return;
        if(pciDeviceHeader->DeviceID == 0xFFFF)
            return;
        
        for(uint64_t function = 0; function < 8; function++) {
            EnumerateFunction(deviceAddress, function);
        }
    }

    void EnumerateBus(uint64_t baseAddress, uint64_t bus) {
        uint64_t offset = bus << 20;
        uint64_t busAddress = baseAddress + offset;
        gPageTableManager.MapMemory((void*)busAddress, (void*)busAddress);

        PCIDeviceHeader* pciDeviceHeader = (PCIDeviceHeader*)busAddress;

        if(pciDeviceHeader->DeviceID == 0)
            return;
        if(pciDeviceHeader->DeviceID == 0xFFFF)
            return;
        
        for(uint64_t device = 0; device < 32; device++) {
            EnumerateDevice(busAddress, device);
        }
    }

    void EnumeratePCI(ACPI::MCFGHeader* mcfg) {
        int entries = ((mcfg->Header.Length) - sizeof(ACPI::MCFGHeader)) / sizeof(ACPI::DeviceConfig);
        for(int i = 0; i< entries; i++) {
            ACPI::DeviceConfig* newDeviceConfig = (ACPI::DeviceConfig*)((uint64_t)mcfg + sizeof(ACPI::MCFGHeader) + (sizeof(ACPI::DeviceConfig) * i));
            for(uint64_t bus = newDeviceConfig->StartBus; bus < newDeviceConfig->EndBus; bus++) {
                EnumerateBus(newDeviceConfig->BaseAddress, bus);
            }
        }
    }
}