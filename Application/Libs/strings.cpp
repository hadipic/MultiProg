#include "strings.h"
#define NL "\n"
Strings::Strings(QObject *parent) :
    QObject(parent)
{
    _language=Language::English;
    STR_ID[S_NL]=(char*) "S_NL";
    STR_ID[S_noprog]=(char*) "S_noprog";
    STR_ID[S_prog]="S_prog";
    STR_ID[S_progDev]="S_progDev";
    STR_ID[S_DevPermission]="S_DevPermission";
    STR_ID[S_nodev_r]="S_nodev_r";
    STR_ID[S_nodev_w]="S_nodev_w";
    STR_ID[S_nodev]="S_nodev";
    STR_ID[S_DevMismatch]="S_DevMismatch";
    STR_ID[S_noV33reg]="S_noV33reg";
    STR_ID[S_progver]="S_progver";
    STR_ID[S_progid]="S_progid";
    STR_ID[S_FWver2old]="S_FWver2old";
    STR_ID[S_lowUsbV]="S_lowUsbV";
    STR_ID[S_HiVPP]="S_HiVPP";
    STR_ID[S_LowVPP]="S_LowVPP";
    STR_ID[S_reg]="S_reg";
    STR_ID[S_dev_det]="S_dev_det";
    STR_ID[S_file]="S_file";
    STR_ID[S_file2]="S_file2";
    STR_ID[S_fileEEP]="S_fileEEP";
    STR_ID[S_noDLL]="S_noDLL";
    STR_ID[S_openEEfile]="S_openEEfile";
    STR_ID[S_saveEEfile]="S_saveEEfile";
    STR_ID[S_IhexShort]="S_IhexShort";
    STR_ID[S_IhexChecksum]="S_IhexChecksum";
    STR_ID[S_InvHandle]="S_InvHandle";
    STR_ID[S_LogFile]="S_LogFile";
    STR_ID[S_FileSaved]="S_FileSaved";
    STR_ID[S_HVICSP]="S_HVICSP";
    STR_ID[S_StartRead]="S_StartRead";
    STR_ID[S_Read]="S_Read";
    STR_ID[S_CodeReading]="S_CodeReading";
    STR_ID[S_CodeReading1]="S_CodeReading1";
    STR_ID[S_CodeReading2]="S_CodeReading2";
    STR_ID[S_ReadEnd]="S_ReadEnd";
    STR_ID[S_ReadID_CONFIG]="S_ReadID_CONFIG";
    STR_ID[S_Read_CONFIG_A]="S_Read_CONFIG_A";
    STR_ID[S_Read_EXE_A]="S_Read_EXE_A";
    STR_ID[S_ReadEE]="S_ReadEE";
    STR_ID[S_StartErase]="S_StartErase";
    STR_ID[S_Writing]="S_Writing";
    STR_ID[S_StartCodeProg]="S_StartCodeProg";
    STR_ID[S_CodeWriting]="S_CodeWriting";
    STR_ID[S_CodeWriting2]="S_CodeWriting2";
    STR_ID[S_IDW]="S_IDW";
    STR_ID[S_ConfigW]="S_ConfigW";
    STR_ID[S_ConfigAreaW]="S_ConfigAreaW";
    STR_ID[S_EEAreaW]="S_EEAreaW";
    STR_ID[S_FuseAreaW]="S_FuseAreaW";
    STR_ID[S_CodeV]="S_CodeV";
    STR_ID[S_CodeV2]="S_CodeV2";
    STR_ID[S_IDV]="S_IDV";
    STR_ID[S_ConfigV]="S_ConfigV";
    STR_ID[S_EEV]="S_EEV";
    STR_ID[S_IntW]="S_IntW";
    STR_ID[S_TotErr]="S_TotErr";
    STR_ID[S_ComplErr]="S_ComplErr";
    STR_ID[S_Compl]="S_Compl";
    STR_ID[S_Ready]="S_Ready";
    STR_ID[S_End]="S_End";
    STR_ID[S_EndErr]="S_EndErr";
    STR_ID[S_ProtErase]="S_ProtErase";
    STR_ID[S_Inohex]="S_Inohex";
    STR_ID[S_ReadErr]="S_ReadErr";
    STR_ID[S_ReadCodeErr]="S_ReadCodeErr";
    STR_ID[S_ReadCodeErr2]="S_ReadCodeErr2";
    STR_ID[S_ReadEEErr]="S_ReadEEErr";
    STR_ID[S_ConfigErr]="S_ConfigErr";
    STR_ID[S_ReadConfigErr]="S_ReadConfigErr";
    STR_ID[S_ErrOsccal]="S_ErrOsccal";
    STR_ID[S_CodeWError]="S_CodeWError";
    STR_ID[S_CodeWError2]="S_CodeWError2";
    STR_ID[S_CodeWError3]="S_CodeWError3";
    STR_ID[S_CodeWError4]="S_CodeWError4";
    STR_ID[S_ConfigWErr]="S_ConfigWErr";
    STR_ID[S_ConfigWErr2]="S_ConfigWErr2";
    STR_ID[S_ConfigWErr3]="S_ConfigWErr3";
    STR_ID[S_ConfigWErr4]="S_ConfigWErr4";
    STR_ID[S_IDErr]="S_IDErr";
    STR_ID[S_ICDErr]="S_ICDErr";
    STR_ID[S_Calib1Err]="S_Calib1Err";
    STR_ID[S_Calib2Err]="S_Calib2Err";
    STR_ID[S_CodeVError]="S_CodeVError";
    STR_ID[S_CodeVError2]="S_CodeVError2";
    STR_ID[S_CodeVError3]="S_CodeVError3";
    STR_ID[S_ErrSing]="S_ErrSing";
    STR_ID[S_ErrPlur]="S_ErrPlur";
    STR_ID[S_MaxErr]="S_MaxErr";
    STR_ID[S_I2CInitErr]="S_I2CInitErr";
    STR_ID[S_I2CAckErr]="S_I2CAckErr";
    STR_ID[S_ComErr]="S_ComErr";
    STR_ID[S_InsErr]="S_InsErr";
    STR_ID[S_SyncErr]="S_SyncErr";
    STR_ID[S_HVregErr]="S_HVregErr";
    STR_ID[S_Log1]="S_Log1";
    STR_ID[S_Log2]="S_Log2";
    STR_ID[S_Log3]="S_Log3";
    STR_ID[S_Log4]="S_Log4";
    STR_ID[S_Log5]="S_Log5";
    STR_ID[S_Log6]="S_Log6";
    STR_ID[S_Log7]="S_Log7";
    STR_ID[S_Log8]="S_Log8";
    STR_ID[S_Log9]="S_Log9";
    STR_ID[S_comTimeout]="S_comTimeout";
    STR_ID[S_EraseTimeout]="S_EraseTimeout";
    STR_ID[S_ConfigTimeout]="S_ConfigTimeout";
    STR_ID[S_ConfigTimeout2]="S_ConfigTimeout2";
    STR_ID[S_CodeTimeout]="S_CodeTimeout";
    STR_ID[S_CodeTimeout2]="S_CodeTimeout2";
    STR_ID[S_OsccalTimeout]="S_OsccalTimeout";
    STR_ID[S_EETimeout]="S_EETimeout";
    STR_ID[S_ConfigWTimeout]="S_ConfigWTimeout";
    STR_ID[S_CodeWTimeout]="S_CodeWTimeout";
    STR_ID[S_ConfigWTimeout2]="S_ConfigWTimeout2";
    STR_ID[S_EEWTimeout]="S_EEWTimeout";
    STR_ID[S_IDWTimeout]="S_IDWTimeout";
    STR_ID[S_ConfigNWTimeout]="S_ConfigNWTimeout";
    STR_ID[S_CodeVTimeout]="S_CodeVTimeout";
    STR_ID[S_ConfigVTimeout]="S_ConfigVTimeout";
    STR_ID[S_ProgModeTimeout]="S_ProgModeTimeout";
    STR_ID[S_ProgModeXTimeout]="S_ProgModeXTimeout";
    STR_ID[S_LogTimeout]="S_LogTimeout";
    STR_ID[S_CodeMem]="S_CodeMem";
    STR_ID[S_EEMem]="S_EEMem";
    STR_ID[S_IDMem]="S_IDMem";
    STR_ID[S_ConfigMem]="S_ConfigMem";
    STR_ID[S_ConfigResMem]="S_ConfigResMem";
    STR_ID[S_CodeMem2]="S_CodeMem2";
    STR_ID[S_ExeMem]="S_ExeMem";
    STR_ID[S_LPOsc]="S_LPOsc";
    STR_ID[S_XTOsc]="S_XTOsc";
    STR_ID[S_IntOsc]="S_IntOsc";
    STR_ID[S_RCOsc]="S_RCOsc";
    STR_ID[S_WDTON]="S_WDTON";
    STR_ID[S_WDTOFF]="S_WDTOFF";
    STR_ID[S_CPOFF]="S_CPOFF";
    STR_ID[S_CPON]="S_CPON";
    STR_ID[S_MCLRON]="S_MCLRON";
    STR_ID[S_MCLROFF]="S_MCLROFF";
    STR_ID[S_ChipID]="S_ChipID";
    STR_ID[S_ChipID2]="S_ChipID2";
    STR_ID[S_BKOsccal]="S_BKOsccal";
    STR_ID[S_Osccal]="S_Osccal";
    STR_ID[S_DevID]="S_DevID";
    STR_ID[S_DevID2]="S_DevID2";
    STR_ID[S_DevREV]="S_DevREV";
    STR_ID[S_ConfigWord]="S_ConfigWord";
    STR_ID[S_ConfigWord2]="S_ConfigWord2";
    STR_ID[S_ConfigWordX]="S_ConfigWordX";
    STR_ID[S_ConfigWordH]="S_ConfigWordH";
    STR_ID[S_ConfigWordL]="S_ConfigWordL";
    STR_ID[S_Config2Cal1]="S_Config2Cal1";
    STR_ID[S_CalibWord1]="S_CalibWord1";
    STR_ID[S_Calib1_2]="S_Calib1_2";
    STR_ID[S_CalibWord2]="S_CalibWord2";
    STR_ID[S_CalibWordX]="S_CalibWordX";
    STR_ID[S_Protected]="S_Protected";
    STR_ID[S_CodeLim]="S_CodeLim";
    STR_ID[S_EELim]="S_EELim";
    STR_ID[S_ConfigLim]="S_ConfigLim";
    STR_ID[S_WbufLim]="S_WbufLim";
    STR_ID[S_MaxRetry]="S_MaxRetry";
    STR_ID[S_NoCode]="S_NoCode";
    STR_ID[S_NoCode2]="S_NoCode2";
    STR_ID[S_NoCalibW]="S_NoCalibW";
    STR_ID[S_NoEEMem]="S_NoEEMem";
    STR_ID[S_NoConfigW]="S_NoConfigW";
    STR_ID[S_NoConfigW2]="S_NoConfigW2";
    STR_ID[S_NoConfigW3]="S_NoConfigW3";
    STR_ID[S_NoConfigW4]="S_NoConfigW4";
    STR_ID[S_NoConfigW5]="S_NoConfigW5";
    STR_ID[S_Empty]="S_Empty";
    STR_ID[S_NextIns]="S_NextIns";
    STR_ID[S_ForceConfigW]="S_ForceConfigW";
    STR_ID[S_ForceConfigW1]="S_ForceConfigW1";
    STR_ID[S_ForceConfigW2]="S_ForceConfigW2";
    STR_ID[I_Fopen]="I_Fopen";
    STR_ID[I_Fsave]="I_Fsave";
    STR_ID[I_DevR]="I_DevR";
    STR_ID[I_DevW]="I_DevW";
    STR_ID[I_Info]="I_Info";
    STR_ID[I_Data]="I_Data";
    STR_ID[I_Opt]="I_Opt";
    STR_ID[I_Dev]="I_Dev";
    STR_ID[I_Type]="I_Type";
    STR_ID[I_Speed]="I_Speed";
    STR_ID[I_ReadRes]="I_ReadRes";
    STR_ID[I_ID_BKo_W]="I_ID_BKo_W";
    STR_ID[I_EE]="I_EE";
    STR_ID[I_CalW]="I_CalW";
    STR_ID[I_OSCW]="I_OSCW";
    STR_ID[I_OSC]="I_OSC";
    STR_ID[I_BKOSC]="I_BKOSC";
    STR_ID[I_OSCF]="I_OSCF";
    STR_ID[I_CONN]="I_CONN";
    STR_ID[I_LOG]="I_LOG";
    STR_ID[I_CK_V33]="I_CK_V33";
    STR_ID[I_LANG]="I_LANG";
    STR_ID[I_MAXERR]="I_MAXERR";
    STR_ID[I_ADDR]="I_ADDR";
    STR_ID[I_USBD]="I_USBD";
    STR_ID[I_I2CDATAOUT]="I_I2CDATAOUT";
    STR_ID[I_I2CDATATR]="I_I2CDATATR";
    STR_ID[I_I2C_NB]="I_I2C_NB";
    STR_ID[I_I2CMode]="I_I2CMode";
    STR_ID[I_I2CSend]="I_I2CSend";
    STR_ID[I_I2CReceive]="I_I2CReceive";
    STR_ID[I_TestHW]="I_TestHW";
    STR_ID[I_TestHWB]="I_TestHWB";
    STR_ID[I_IO_Enable]="I_IO_Enable";
    STR_ID[I_PIC_CONFIG]="I_PIC_CONFIG";
    STR_ID[I_PIC_FORCECW]="I_PIC_FORCECW";
    STR_ID[I_AT_CONFIG]="I_AT_CONFIG";
    STR_ID[I_AT_FUSE]="I_AT_FUSE";
    STR_ID[I_AT_FUSEH]="I_AT_FUSEH";
    STR_ID[I_AT_FUSEX]="I_AT_FUSEX";
    STR_ID[I_AT_LOCK]="I_AT_LOCK";
    STR_ID[I_W_LANGFILE]="I_W_LANGFILE";
    STR_ID[I_WAITS1]="I_WAITS1";
    STR_ID[I_PRESSS1]="I_PRESSS1";
    STR_ID[S_WaitS1W]="S_WaitS1W";
    STR_ID[S_WaitS1R]="S_WaitS1R";
    STR_ID[I_ICD_ENABLE]="I_ICD_ENABLE";
    STR_ID[I_ICD_ADDRESS]="I_ICD_ADDRESS";
    STR_ID[I_LOAD_COFF]="I_LOAD_COFF";
    STR_ID[I_SHOW_PC]="I_SHOW_PC";
    STR_ID[I_SHOW_STATUS]="I_SHOW_STATUS";
    STR_ID[I_SHOW_BANK0]="I_SHOW_BANK0";
    STR_ID[I_SHOW_BANK1]="I_SHOW_BANK1";
    STR_ID[I_SHOW_BANK2]="I_SHOW_BANK2";
    STR_ID[I_SHOW_BANK3]="I_SHOW_BANK3";
    STR_ID[I_SHOW_EE]="I_SHOW_EE";
    STR_ID[I_ICD_RUN]="I_ICD_RUN";
    STR_ID[I_ICD_HALT]="I_ICD_HALT";
    STR_ID[I_ICD_STEP]="I_ICD_STEP";
    STR_ID[I_ICD_STEPOVER]="I_ICD_STEPOVER";
    STR_ID[I_ICD_STOP]="I_ICD_STOP";
    STR_ID[I_ICD_REFRESH]="I_ICD_REFRESH";
    STR_ID[I_ICD_CMD]="I_ICD_CMD";
    STR_ID[I_ICD_HELP]="I_ICD_HELP";
    STR_ID[I_ICD_SOURCE]="I_ICD_SOURCE";
    STR_ID[I_ICD_STATUS]="I_ICD_STATUS";
    STR_ID[I_ICD_HELP_TXT]="I_ICD_HELP_TXT";
    STR_ID[I_GUI_CMD_HELP]="I_GUI_CMD_HELP";
    STR_ID[L_HELP]="L_HELP";
    STR_ID[L_OPTERR]="L_OPTERR";
    STR_ID[L_INFO1]="L_INFO1";
    STR_ID[L_INFO2]="L_INFO2";
    STR_ID[L_UNKNOWN]="L_UNKNOWN";
    STR_ID[L_NAME]="L_NAME";
    STR_ID[L_DEV_RO]="L_DEV_RO";
    STR_ID[L_DEV_RW]="L_DEV_RW";
    //	#endif

    // ENGLISH strings
    strings_en[S_NL]=NL;
    strings_en[S_noprog]="Programmer not detected" NL;
    strings_en[S_prog]="Programmer detected" NL;
    strings_en[S_progDev]="Programmer detected on %s" NL;
    strings_en[S_DevPermission]="Cannot open %s, make sure you have read permission on it" NL;
    strings_en[S_nodev_r]="Device not supported for reading" NL;
    strings_en[S_nodev_w]="Device not supported for writing" NL;
    strings_en[S_nodev]="Unknown device" NL;
    strings_en[S_DevMismatch]="Warning: the device is different from what specified in source data" NL;
    strings_en[S_noV33reg]="3.3V Regulator not detected" NL;
    strings_en[S_progver]="Firmware version %d.%d.%d" NL;
    strings_en[S_progid]="Hardware ID: %d.%d.%d";
    strings_en[S_FWver2old]="This firmware is too old. Version %s is required" NL;
    strings_en[S_lowUsbV]="USB voltage too low (VUSB<4.5V)" NL;
    strings_en[S_HiVPP]="Warning: regulator voltage too high" NL NL;
    strings_en[S_LowVPP]="Warning: regulator voltage too low" NL NL;
    strings_en[S_reg]="Regulator up and running after %dms VPP=%.1fV" NL NL;
    strings_en[S_dev_det]="Device detected: vid=0x%04X pid=0x%04X" NL "Path: %s" NL;
    strings_en[S_file]="Hex8 files (*.hex)|*.hex|All files (*.*)|*.*||";
    strings_en[S_file2]="Hex8 files (*.hex)|*.hex|Binary files (*.bin)|*.bin|All files (*.*)|*.*||";
    strings_en[S_fileEEP]="Hex8 files (*.hex *.eep)|*.hex;*.eep|All files (*.*)|*.*||";
    strings_en[S_noDLL]="Error opening hid.dll";
    strings_en[S_openEEfile]="Open EEPROM file";
    strings_en[S_saveEEfile]="Save EEPROM file";
    //
    strings_en[S_IhexShort]="Intel hex8 line too short:" NL "%s" NL;
    strings_en[S_IhexChecksum]="Intel hex8 checksum error in line:" NL "%s" NL;
    strings_en[S_InvHandle]="Invalid handle" NL;
    strings_en[S_LogFile]="Log.txt";
    strings_en[S_FileSaved]="Saved file %s" NL;
    //
    strings_en[S_HVICSP]="High Voltage ICSP" NL;
    strings_en[S_StartRead]="Reading ..." NL;
    strings_en[S_Read]="Reading ... ";
    strings_en[S_CodeReading1]="Reading code ... ";
    strings_en[S_CodeReading]="Reading: %3d%%, add. %03X";
    strings_en[S_CodeReading2]="Reading: %3d%%, add. %05X";
    strings_en[S_ReadEnd]="Reading completed: %d words" NL;
    strings_en[S_ReadID_CONFIG]="Reading ID and CONFIG";
    strings_en[S_Read_CONFIG_A]="Reading CONFIG area ... ";
    strings_en[S_Read_EXE_A]="Reading Executive area ... ";
    strings_en[S_ReadEE]="Reading EEPROM ... ";
    strings_en[S_StartErase]="Erasing ... ";
    strings_en[S_Writing]="Writing ..." NL;
    strings_en[S_StartCodeProg]="Writing code ... ";
    strings_en[S_CodeWriting]="Writing: %3d%%, add. %03X";
    strings_en[S_CodeWriting2]="Writing: %3d%%, add. %04X";
    strings_en[S_IDW]="Writing ID ... ";
    strings_en[S_ConfigW]="Writing CONFIG ... ";
    strings_en[S_ConfigAreaW]="Writing CONFIG area ... ";
    strings_en[S_EEAreaW]="Writing EEPROM ... ";
    strings_en[S_FuseAreaW]="Writing FUSE area ... ";
    strings_en[S_CodeV]="Verifying code ... ";
    strings_en[S_CodeV2]="Verifying: %d%%, add. %04X";
    strings_en[S_IDV]="Verifying ID ... ";
    strings_en[S_ConfigV]="Verifying CONFIG ... ";
    strings_en[S_EEV]="Verifying EEPROM ... ";
    strings_en[S_IntW]="Write interrupted" NL;
    strings_en[S_TotErr]="completed, %d total errors" NL;
    strings_en[S_ComplErr]="completed, %d errors" NL;
    strings_en[S_Compl]="completed" NL;
    strings_en[S_Ready]="ready";
    strings_en[S_End]= NL "End (%.2f s)" NL NL;
    strings_en[S_EndErr]= NL "End (%.2f s) %d %s" NL NL;
    strings_en[S_ProtErase]="Device protected, overriding protection." NL;
    //
    strings_en[S_Inohex]="Error: '%.4s' doesn't look very hexadecimal, right?" NL;
    strings_en[S_ReadErr]="Read error: requested words=%d, read=%d" NL;
    strings_en[S_ReadCodeErr]="Error reading code area, requested %d words, read %d" NL;
    strings_en[S_ReadCodeErr2]="Error reading code area, requested %d bytes, read %d" NL;
    strings_en[S_ReadEEErr]="Error reading EEPROM area, requested %d bytes, read %d" NL;
    strings_en[S_ConfigErr]="Error reading configuration area, requested %d words, read %d" NL;
    strings_en[S_ReadConfigErr]="Error reading configuration area, requested %d bytes, read %d" NL;
    strings_en[S_ErrOsccal]="Error reading OSCCAL and BKOSCCAL";
    strings_en[S_CodeWError]="Error writing address %3X: written %03X, read %03X" NL;
    strings_en[S_CodeWError2]="Error writing address %3X: written %04X, read %04X" NL;
    strings_en[S_CodeWError3]="Error writing address %4X: written %02X, read %02X" NL;
    strings_en[S_CodeWError4]="Error writing code area, requested %d bytes, written %d" NL;
    strings_en[S_ConfigWErr]="Error writing config area: written %03X, read %03X" NL;
    strings_en[S_ConfigWErr2]="Error writing CONFIG";
    strings_en[S_ConfigWErr3]="Error writing config area: written %04X, read %04X" NL;
    strings_en[S_ConfigWErr4]="Error writing %s: written %02X, read %02X" NL;
    strings_en[S_IDErr]="Error writing ID%d: written %04X, read %04X" NL;
    strings_en[S_ICDErr]="Error writing ICD (0x%X): written %04X, read %04X" NL;
    strings_en[S_Calib1Err]="Error writing Calib1: written %04X, read %04X" NL;
    strings_en[S_Calib2Err]="Error writing Calib2: written %04X, read %04X" NL;
    strings_en[S_CodeVError]="Error verifying address %04X (%d), written %02X, read %02X" NL;
    strings_en[S_CodeVError2]="Error verifying code area, requested %d bytes, read %d" NL;
    strings_en[S_CodeVError3]="Error verifying code area, requested %d words, read %d" NL;
    strings_en[S_ErrSing]="error";
    strings_en[S_ErrPlur]="errors";
    strings_en[S_MaxErr]="Exceeded maximum number of errors (%d), write interrupted" NL;
    strings_en[S_I2CInitErr]="Error initializing I2C bus" NL;
    strings_en[S_I2CAckErr]="I2C acknowledge error" NL;
    strings_en[S_ComErr]="Communication error" NL;
    strings_en[S_InsErr]="Unknown instruction" NL;
    strings_en[S_SyncErr]="Synchronization error" NL;
    strings_en[S_HVregErr]="HV regulator error" NL;
    //
    strings_en[S_Log1]="i=%d(0x%X), k=%d(0x%X)  NumberOfBytesRead=%d" NL;
    strings_en[S_Log2]="i=%d, k=%d, errors=%d, NumberOfBytesRead=%d" NL NL;
    strings_en[S_Log3]="i=%d, k2=%d NumberOfBytesRead=%d" NL;
    strings_en[S_Log4]="Config area. 	errors=%d   NumberOfBytesRead=%d" NL;
    strings_en[S_Log5]="dim=%d(0x%X), dimx=%d(0x%X), dimx/wbuf=%d " NL NL;
    strings_en[S_Log6]="i=%d(0x%X),  NumberOfBytesRead=%d" NL NL;
    strings_en[S_Log7]="i=%d(0x%X), k=%d(0x%X)" NL;
    strings_en[S_Log8]="i=%d(0x%X), k=%d(0x%X), errors=%d" NL;
    strings_en[S_Log9]="Config area. 	errors=%d" NL;
    //
    strings_en[S_comTimeout]="Communication timeout" NL;
    strings_en[S_EraseTimeout]="Timeout during erase" NL;
    strings_en[S_ConfigTimeout]="Timeout reading CONFIG" NL;
    strings_en[S_ConfigTimeout2]="Timeout reading config area" NL;
    strings_en[S_CodeTimeout]="Timeout reading address %02X (%d)" NL;
    strings_en[S_CodeTimeout2]="Timeout reading address %04X (%d)" NL;
    strings_en[S_OsccalTimeout]="Timeout reading osccal" NL;
    strings_en[S_EETimeout]="Timeout reading EEPROM, address %02X (%d)" NL;
    strings_en[S_ConfigWTimeout]="Timeout verifying config" NL;
    strings_en[S_CodeWTimeout]="Timeout writing address %02X (%d)" NL;
    strings_en[S_ConfigWTimeout2]="Timeout writing config area" NL;
    strings_en[S_EEWTimeout]="Timeout writing EEPROM, address %03X (%d)" NL;
    strings_en[S_IDWTimeout]="Timeout writing ID" NL;
    strings_en[S_ConfigNWTimeout]="Timeout writing CONFIG%d" NL;
    strings_en[S_CodeVTimeout]="Timeout verifying address %04X (%d)" NL;
    strings_en[S_ConfigVTimeout]="Timeout verifying config area" NL;
    strings_en[S_ProgModeTimeout]="Timeout entering program mode" NL;
    strings_en[S_ProgModeXTimeout]="Timeout exiting program mode" NL;
    strings_en[S_LogTimeout]="Timeout" NL;
    //
    strings_en[S_CodeMem]= NL "Code memory:" NL;
    strings_en[S_EEMem]= NL "EEPROM memory:" NL;
    strings_en[S_IDMem]= NL "ID:" NL;
    strings_en[S_ConfigMem]= NL "CONFIG:" NL;
    strings_en[S_ConfigResMem]= NL "Configuration and reserved memory:" NL;
    strings_en[S_CodeMem2]= NL "Code memory:" NL;
    strings_en[S_ExeMem]= NL "Executive memory:" NL;
    //
    strings_en[S_LPOsc]="LP oscillator" NL;
    strings_en[S_XTOsc]="XT oscillator" NL;
    strings_en[S_IntOsc]="Internal osc." NL;
    strings_en[S_RCOsc]="RC oscillator" NL;
    strings_en[S_WDTON]="WDT ON" NL;
    strings_en[S_WDTOFF]="WDT OFF" NL;
    strings_en[S_CPOFF]="Code protection OFF" NL;
    strings_en[S_CPON]="Code protection ON" NL;
    strings_en[S_MCLRON]="Master clear ON" NL;
    strings_en[S_MCLROFF]="Master clear OFF" NL;
    strings_en[S_ChipID]="ID%d: 0x%03X   ID%d: 0x%03X" NL;
    strings_en[S_ChipID2]="ID%d: 0x%02X   ID%d: 0x%02X" NL;
    strings_en[S_BKOsccal]="Backup OSCCAL: 0x%03X" NL;
    strings_en[S_Osccal]="OSCCAL: 0x%03X" NL;
    strings_en[S_DevID]="DevID: 0x%04X" NL;
    strings_en[S_DevID2]="DevID: 0x%02X%02X" NL;
    strings_en[S_DevREV]="DevREV: 0x%04X" NL;
    strings_en[S_ConfigWord]="Configuration word: 0x%03X" NL;
    strings_en[S_Config2Cal1]="Configuration word2 or calibration word 1: 0x%03X" NL;
    strings_en[S_ConfigWord2]="Configuration word2: 0x%04X" NL;
    strings_en[S_ConfigWordX]="Configuration word %d: 0x%04X" NL;
    strings_en[S_ConfigWordH]="CONFIG%dH: 0x%02X\t";
    strings_en[S_ConfigWordL]="CONFIG%dL: 0x%02X" NL;
    strings_en[S_CalibWord1]="Calibration word1: 0x%04X" NL;
    strings_en[S_Calib1_2]="Calibration word 1 or 2: 0x%04X" NL;
    strings_en[S_CalibWord2]="Calibration word2: 0x%04X" NL;
    strings_en[S_CalibWordX]="Calibration word %d: 0x%04X" NL;
    strings_en[S_Protected]="protected device";
    //
    strings_en[S_CodeLim]="Code size exceeds limits" NL;
    strings_en[S_EELim]="Eeprom size exceeds limits" NL;
    strings_en[S_ConfigLim]="Config area size exceeds limits" NL;
    strings_en[S_WbufLim]="Write buffer size exceeds limits" NL;
    strings_en[S_MaxRetry]="Max retries in writing: %d" NL;
    //
    strings_en[S_NoCode]="Data area is empty" NL;
    strings_en[S_NoCode2]="Nothing to write" NL;
    strings_en[S_NoCalibW]="Can't find calibration data" NL;
    strings_en[S_NoEEMem]="Can't find EEPROM data" NL;
    strings_en[S_NoConfigW]="Impossibile leggere la config word" NL;
    strings_en[S_NoConfigW2]="Can't find CONFIG location (0xFFF)" NL;
    strings_en[S_NoConfigW3]="Can't find CONFIG location (0x2007)" NL "End" NL;
    strings_en[S_NoConfigW4]="Can't find CONFIG location (0x2008)" NL "End" NL;
    strings_en[S_NoConfigW5]="Can't find CONFIG location (0x8007-0x8008)" NL "End" NL;
    strings_en[S_Empty]="(empty)" NL;
    strings_en[S_NextIns]="Next instruction";
    strings_en[S_ForceConfigW]="Forcing config word1 (0x%04X) and config word2 (0x%04X)" NL;
    strings_en[S_ForceConfigW1]="Forcing config word1 (0x%04X)" NL;
    strings_en[S_ForceConfigW2]="Forcing config word2 (0x%04X)" NL;
    //
    strings_en[I_Fopen]="Open file";
    strings_en[I_Fsave]="Save file";
    strings_en[I_DevR]="Read device";
    strings_en[I_DevW]="Write device";
    strings_en[I_Info]="Info";
    strings_en[I_Data]="Data";
    strings_en[I_Opt]="Options";
    strings_en[I_Dev]="Device";
    strings_en[I_Type]="Filter by type";
    strings_en[I_Speed]="Speed";
    strings_en[I_ReadRes]="Read reserved area";
    strings_en[I_ID_BKo_W]="Write ID and BKosccal";
    strings_en[I_EE]="Read and write EEPROM";
    strings_en[I_CalW]="Write Calib1 e 2";
    strings_en[I_OSCW]="Write OSCCal";
    strings_en[I_OSC]="OSCCal";
    strings_en[I_BKOSC]="Backup OSCCal";
    strings_en[I_OSCF]="From File";
    strings_en[I_CONN]="Reconnect";
    strings_en[I_LOG]="Log activity";
    strings_en[I_CK_V33]="Don't require LV boards";
    strings_en[I_LANG]="Language";
    strings_en[I_MAXERR]="Max errors during write";
    strings_en[I_ADDR]="Address";
    strings_en[I_USBD]="Min USB delay (ms)";
    strings_en[I_I2CDATAOUT]="Data to send";
    strings_en[I_I2CDATATR]="Data transferred";
    strings_en[I_I2C_NB]="Bytes to read/write";
    strings_en[I_I2CMode]="Mode";
    strings_en[I_I2CSend]="Send";
    strings_en[I_I2CReceive]="Receive";
    strings_en[I_TestHW]="Hardware test: remove any device from programmer";
    strings_en[I_TestHWB]="Hardware test";
    strings_en[I_IO_Enable]="Enable IO";
    strings_en[I_PIC_CONFIG]="PIC configuration";
    strings_en[I_PIC_FORCECW]="Force config word";
    strings_en[I_AT_CONFIG]="Atmel configuration";
    strings_en[I_AT_FUSE]="Write Fuse Low";
    strings_en[I_AT_FUSEH]="Write Fuse High";
    strings_en[I_AT_FUSEX]="Write Extended Fuse";
    strings_en[I_AT_LOCK]="Write Lock";
    strings_en[I_W_LANGFILE]="Write language file";
    strings_en[I_WAITS1]="Wait for S1 before read/write";
    strings_en[I_PRESSS1]="Press S1 to start" NL;
    strings_en[S_WaitS1W]="Press S1 to program, any key to exit";
    strings_en[S_WaitS1R]="Press S1 to read, any key to exit";
    strings_en[I_ICD_ENABLE]="Enable ICD";
    strings_en[I_ICD_ADDRESS]="ICD routine address";
    strings_en[I_LOAD_COFF]="Load COFF file ...";
    strings_en[I_SHOW_PC]="Show Program Counter";
    strings_en[I_SHOW_STATUS]="Show status registers";
    strings_en[I_SHOW_BANK0]="Show memory bank 0";
    strings_en[I_SHOW_BANK1]="Show memory bank 1";
    strings_en[I_SHOW_BANK2]="Show memory bank 2";
    strings_en[I_SHOW_BANK3]="Show memory bank 3";
    strings_en[I_SHOW_EE]="show EEPROM";
    strings_en[I_ICD_RUN]="Run/Continue";
    strings_en[I_ICD_HALT]="Halt";
    strings_en[I_ICD_STEP]="Step";
    strings_en[I_ICD_STEPOVER]="Step over";
    strings_en[I_ICD_STOP]="Stop";
    strings_en[I_ICD_REFRESH]="Refresh";
    strings_en[I_ICD_CMD]="Command line";
    strings_en[I_ICD_HELP]="Help";
    strings_en[I_ICD_SOURCE]="Source";
    strings_en[I_ICD_STATUS]="Status";
    strings_en[I_ICD_HELP_TXT]=
            "ICD help" NL NL
            "Double click variable name to add to watch list;" NL
            "double click again to remove it." NL
            "Key shortcuts:" NL
            " F1 help" NL
            " F5 halt" NL
            " F7 step" NL
            " F8 step over" NL
            " F9 run" NL
            "Command-line help:" NL
            " variable=x          set variable to x" NL
            " [var addr]=x        set variable at addr to x" NL
            " help                command help" NL
            " break <addr>        set breakpoint at address <addr>" NL
            " freeze [on,off]     freeze peripherals" NL
            " h[alt]              halt execution" NL
            " list <addr>         show code starting from <addr>" NL
            " p[rint] 0x<addr>    print variable at address <addr>" NL
            " p[rint] <var>       print variable <var>" NL
            " p[rint] bank <b>    print registers in bank <b>" NL
            " p[rint] p <addr>    print program memory at address <addr>" NL
            " p[rint] ee <addr>   print eeprom memory at address <addr>" NL
            " p[rint] ee          print all eeprom memory" NL
            " r[un]               run" NL
            " s[tep] [n]          single step [n times]" NL
            " step over / ss [n]  step over calls [n times]" NL
            " version             read debugger version" NL
            " w[atch] variable    add/remove watch for variable" NL
            " w[atch] 0x<addr>    add/remove watch at address <addr>" NL;
    strings_en[I_GUI_CMD_HELP]=
            "Command line options:" NL
            " ?,h,help            this help" NL
            " command <message>   send <message> to the programmer and exit." NL
            "                     The message is composed of up to 64 hexadecimal bytes separated by a space;" NL
            "                     those not specified are 0" NL
            " lang <language>     load <language> strings" NL
            " langfile            write all strings to file" NL;
    strings_en[L_HELP]="op [options]" NL
            "-BKosccal                   load BKosccal from file" NL
            "-calib                      load calibration from file" NL
            "-command <message>          send <message> to the programmer and exit." NL
            "                            The message is composed of up to 64 hexadecimal bytes separated by a space;" NL
            "                            those not specified are 0" NL
            "-cw1 <cw1>                  force config word 1" NL
            "-cw2 <cw2>                  force config word 2" NL
            "-d, device <dev.>           device" NL
            "-ee                         use eeprom" NL
            "-err <max>                  max errors during writing" NL
            "-fuse <val>                 write fuse low byte (Atmel only)" NL
            "-fuseh <val>                write fuse high byte (Atmel only)" NL
            "-fusex <val>                write extended fuse byte (Atmel only)" NL
            "-h, help                    help" NL
            "-HWtest                     hardware test" NL
            "-i, info                    informations about programmer" NL
            "-i2c_r <N Ctr Addr>         read N bytes from I2C bus" NL
            "-i2c_r2 <N Ctr Addr(2)>     read N bytes from I2C bus (16b address)" NL
            "-i2c_w <N Ctr Addr Data>    write N bytes to I2C bus" NL
            "-i2c_w2 <N Ctr Addr(2) D.>  write N bytes to I2C bus (16b address)" NL
            "-i2cspeed <s>               set I2C speed: 0=100k,1=200k,2=500k,3=800k" NL
            "-id                         use ID" NL
            "-icd <val>                  enable ICD (goto address)" NL
            "-l, log [=file]             save log" NL
            "-lang <language>            load <language> strings" NL
            "-langfile                   write all strings to file" NL
            "-lock <val>                 write lock byte (Atmel only)" NL
            "-mode <mode>                SPI mode: 00,01,10,11" NL
            "-nolvcheck                  don't require LV boards" NL
            "-osccal                     loads osccal from file instead of using the value saved before erase" NL
            "-p, path <path>             programmer path [/dev/usb/hiddev0]" NL
            "-pid <pid>                  programmer pid [0x100]" NL
            "-r, reserved                read reserved area" NL
            "-rep <n>                    report size [64]" NL
            "-s1, S1                     Multiple programming triggered by S1" NL
            "-s, save <file>             save Ihex file" NL
            "-se, saveEE <file>          save EEPROM on Ihex file (ATxxxx only)" NL
            "-spi_r <N>                  read N bytes from SPI bus" NL
            "-spi_w <N Data>             write N bytes to SPI bus" NL
            "-spispeed <s>               set SPI speed: 0=100k,1=200k,2=500k,3=800k" NL
            "-support                    supported devices" NL
            "-use_BKosccal               copy BKosccal to osccal" NL
            "-v, version                 version" NL
            "-vid <vid>                  programmer vid [0x4D8]" NL
            "-w, write <file>            write Ihex file" NL
            "-we, writeEE <file>         write EEPROM on Ihex file (ATxxxx only)" NL
            "" NL
            "ex.  op -d 12F683 -s test.hex" NL;
    strings_en[L_OPTERR]="Error in command-line options" NL;
    strings_en[L_INFO1]="vid 0x%04hx pid 0x%04hx version 0x%04hx ";
    strings_en[L_INFO2]= NL "bus: %d devnum: %d ifnum: %d" NL;
    strings_en[L_UNKNOWN]="Unknown";
    strings_en[L_NAME]="The device on %s says its name is %s" NL;
    strings_en[L_DEV_RO]="Devices supported for reading only:";
    strings_en[L_DEV_RW]="Devices supported for reading and writing:";
}

const QString& Strings::operator[] (const size_t index)
{
    if(_language==Language::English)
        return strings_en[index];
    else
        return strings_fa[index];
}

Language::Language Strings::language()
{
    return this->_language;
}


void Strings::setLanguage(Language::Language language)
{
    this->_language = language;
}
