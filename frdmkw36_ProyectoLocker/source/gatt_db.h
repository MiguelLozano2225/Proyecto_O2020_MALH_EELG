PRIMARY_SERVICE(service_gatt, gBleSig_GenericAttributeProfile_d)
        CHARACTERISTIC(char_service_changed, gBleSig_GattServiceChanged_d, (gGattCharPropIndicate_c) )
            VALUE(value_service_changed, gBleSig_GattServiceChanged_d, (gPermissionNone_c), 4, 0x00, 0x00, 0x00, 0x00)
            CCCD(cccd_service_changed)

PRIMARY_SERVICE(service_gap, gBleSig_GenericAccessProfile_d)
    CHARACTERISTIC(char_device_name, gBleSig_GapDeviceName_d, (gGattCharPropRead_c) )
            VALUE(value_device_name, gBleSig_GapDeviceName_d, (gPermissionFlagReadable_c), 9, "LabLocker")
    CHARACTERISTIC(char_appearance, gBleSig_GapAppearance_d, (gGattCharPropRead_c) )
            VALUE(value_appearance, gBleSig_GapAppearance_d, (gPermissionFlagReadable_c), 2, UuidArray(gUnknown_c))
    CHARACTERISTIC(char_ppcp, gBleSig_GapPpcp_d, (gGattCharPropRead_c) )
            VALUE(value_ppcp, gBleSig_GapPpcp_d, (gPermissionFlagReadable_c), 8, 0x0A, 0x00, 0x10, 0x00, 0x64, 0x00, 0xE2, 0x04)

PRIMARY_SERVICE_UUID128(LockerKey_service, uuid_LockerKey_service)
    CHARACTERISTIC_UUID128(AccessKey_char, uuid_AccessKey_char, (gGattCharPropWrite_c | gGattCharPropRead_c))
		VALUE_UUID128_VARLEN(AccessKey_value, uuid_AccessKey_char, (gPermissionFlagWritable_c | gPermissionFlagReadable_c), 5, 1)
	CHARACTERISTIC_UUID128(MaterialKey_char, uuid_MaterialKey_char, (gGattCharPropWrite_c | gGattCharPropRead_c))
		VALUE_UUID128_VARLEN(MaterialKey_value, uuid_MaterialKey_char, (gPermissionFlagWritable_c | gPermissionFlagReadable_c), 6, 1)
	CHARACTERISTIC_UUID128(Name_char, uuid_Name_char, (gGattCharPropWrite_c | gGattCharPropRead_c))
		VALUE_UUID128_VARLEN(Name_value, uuid_Name_char, (gPermissionFlagWritable_c | gPermissionFlagReadable_c), 17, 1)
	CHARACTERISTIC_UUID128(AvailableA_char, uuid_AvailableA_char, (gGattCharPropWrite_c | gGattCharPropRead_c))
		VALUE_UUID128_VARLEN(AvailableA_value, uuid_AvailableA_char, (gPermissionFlagWritable_c | gPermissionFlagReadable_c), 1, 1)
	CHARACTERISTIC_UUID128(AvailableB_char, uuid_AvailableB_char, (gGattCharPropWrite_c | gGattCharPropRead_c))
		VALUE_UUID128_VARLEN(AvailableB_value, uuid_AvailableB_char, (gPermissionFlagWritable_c | gPermissionFlagReadable_c), 1, 1)
	CHARACTERISTIC_UUID128(AvailableC_char, uuid_AvailableC_char, (gGattCharPropWrite_c | gGattCharPropRead_c))
		VALUE_UUID128_VARLEN(AvailableC_value, uuid_AvailableC_char, (gPermissionFlagWritable_c | gPermissionFlagReadable_c), 1, 1)
	CHARACTERISTIC_UUID128(Dummy_char, uuid_Dummy_char, (gGattCharPropWrite_c | gGattCharPropRead_c))
		VALUE_UUID128_VARLEN(Dummy_value, uuid_Dummy_char, (gPermissionFlagWritable_c | gPermissionFlagReadable_c), 1, 1)

PRIMARY_SERVICE(service_device_info, gBleSig_DeviceInformationService_d)
    CHARACTERISTIC(char_manuf_name, gBleSig_ManufacturerNameString_d, (gGattCharPropRead_c) )
        VALUE(value_manuf_name, gBleSig_ManufacturerNameString_d, (gPermissionFlagReadable_c), sizeof(MANUFACTURER_NAME)-1, MANUFACTURER_NAME)
    CHARACTERISTIC(char_model_no, gBleSig_ModelNumberString_d, (gGattCharPropRead_c) )
        VALUE(value_model_no, gBleSig_ModelNumberString_d, (gPermissionFlagReadable_c), 9, "LabLocker")
    CHARACTERISTIC(char_serial_no, gBleSig_SerialNumberString_d, (gGattCharPropRead_c) )
        VALUE(value_serial_no, gBleSig_SerialNumberString_d, (gPermissionFlagReadable_c), 7, "BLESN01")
    CHARACTERISTIC(char_hw_rev, gBleSig_HardwareRevisionString_d, (gGattCharPropRead_c) )
        VALUE(value_hw_rev, gBleSig_HardwareRevisionString_d, (gPermissionFlagReadable_c), sizeof(BOARD_NAME)-1, BOARD_NAME)
    CHARACTERISTIC(char_fw_rev, gBleSig_FirmwareRevisionString_d, (gGattCharPropRead_c) )
        VALUE(value_fw_rev, gBleSig_FirmwareRevisionString_d, (gPermissionFlagReadable_c), 5, "1.1.1")
    CHARACTERISTIC(char_sw_rev, gBleSig_SoftwareRevisionString_d, (gGattCharPropRead_c) )
        VALUE(value_sw_rev, gBleSig_SoftwareRevisionString_d, (gPermissionFlagReadable_c), 5, "1.1.1")
    CHARACTERISTIC(char_system_id, gBleSig_SystemId_d, (gGattCharPropRead_c) )
        VALUE(value_system_id, gBleSig_SystemId_d, (gPermissionFlagReadable_c), 8, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0x9F, 0x04, 0x00)
    CHARACTERISTIC(char_rcdl, gBleSig_IeeeRcdl_d, (gGattCharPropRead_c) )
        VALUE(value_rcdl, gBleSig_IeeeRcdl_d, (gPermissionFlagReadable_c), 4, 0x00, 0x00, 0x00, 0x00)
