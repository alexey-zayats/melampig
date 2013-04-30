;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  ;Name and file
  Name "Meampig"
  OutFile "Melampig.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\Melampig"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Melampig" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel user

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\Melampig" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

;--------------------------------
;Pages

; !insertmacro MUI_PAGE_LICENSE ""
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English" ;first language is the default language
  !insertmacro MUI_LANGUAGE "Russian"

;--------------------------------
;Reserve Files
  
  ;If you are using solid compression, files that are required before
  ;the actual installation should be stored first in the data block,
  ;because this will make your installer start faster.
  
  !insertmacro MUI_RESERVEFILE_LANGDLL

!include LocalConfig.nsi

!define PRODUCT_NAME "Melampig"
!define PRODUCT_VERSION "1.0.0"
!define PRODUCT_WEB_SITE "http://agitum.ru/melampig"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

;Installer Sections

; Base libraries
Section "-base"
  
  SetOutPath $INSTDIR\bin
  File "${PG}\lib\libpq.dll"
  File "${PG}\bin\SSLEAY32.dll"
  File "${PG}\bin\LIBEAY32.dll"
  File "${PG}\bin\libintl.dll"
  File "${PG}\bin\libiconv.dll"
  
  File ${QT}\bin\QtCore4.dll
  File ${QT}\bin\QtGui4.dll
  File ${QT}\bin\QtNetwork4.dll
  File ${QT}\bin\QtSql4.dll
  File ${QT}\bin\QtSvg4.dll
  File ${QT}\bin\QtXml4.dll
  File ${QT}\bin\phonon4.dll
  File ${QT}\bin\QtWebKit4.dll
  
  SetOutPath "$INSTDIR\bin\sqldrivers"
  File ${QT}\plugins\sqldrivers\qsqlpsql4.dll
  
  SetOutPath "$INSTDIR\bin\imageformats"
  File ${QT}\plugins\imageformats\qgif4.dll
  File ${QT}\plugins\imageformats\qico4.dll
  File ${QT}\plugins\imageformats\qjpeg4.dll
  File ${QT}\plugins\imageformats\qmng4.dll
  File ${QT}\plugins\imageformats\qsvg4.dll
  File ${QT}\plugins\imageformats\qtiff4.dll
  
  SetOutPath "$INSTDIR\bin\bearer"
  File ${QT}\plugins\bearer\qgenericbearer4.dll
  File ${QT}\plugins\bearer\qnativewifibearer4.dll
  
  SetOutPath "$INSTDIR\bin\codecs"
  File ${QT}\plugins\codecs\qcncodecs4.dll
  File ${QT}\plugins\codecs\qjpcodecs4.dll
  File ${QT}\plugins\codecs\qkrcodecs4.dll
  File ${QT}\plugins\codecs\qtwcodecs4.dll
  
  SetOutPath "$INSTDIR"
  File ${MELAMPIG}\icons\manager.ico 

  WriteUninstaller "$INSTDIR\uninstall.exe"
SectionEnd

Section "Melampig" SecManager
  SetOutPath "$INSTDIR\bin"
  File ${MELAMPIG}\bin\melampig.exe 
  File ${MELAMPIG}\bin\orm.dll 
  File ${MELAMPIG}\bin\ui.dll
  
  SetOutPath "$INSTDIR\lib"
  File ${MELAMPIG}\lib\competition.dll
  File ${MELAMPIG}\lib\reference.dll
  File ${MELAMPIG}\lib\reports.dll

  SetOutPath "$INSTDIR\share\translations\ru"
  File ${MELAMPIG}\share\translations\ru\qt.qm
  File ${MELAMPIG}\share\translations\ru\melampig.qm
  File ${MELAMPIG}\share\translations\ru\orm.qm
  File ${MELAMPIG}\share\translations\ru\ui.qm
  File ${MELAMPIG}\share\translations\ru\competition.qm
  File ${MELAMPIG}\share\translations\ru\reference.qm
  File ${MELAMPIG}\share\translations\ru\reports.qm
  
  SetOutPath "$INSTDIR\share\translations\en"
  File ${MELAMPIG}\share\translations\en\melampig.qm
  File ${MELAMPIG}\share\translations\en\orm.qm
  File ${MELAMPIG}\share\translations\en\ui.qm
  File ${MELAMPIG}\share\translations\en\competition.qm
  File ${MELAMPIG}\share\translations\en\reference.qm
  File ${MELAMPIG}\share\translations\en\reports.qm
  
  SetOutPath "$INSTDIR\share\img"
  File ${MELAMPIG}\share\img\fila-logo.png
  File ${MELAMPIG}\share\img\fsw-logo.png
  
  !include flags-in.nsi
  !include flags2-in.nsi
  !include reports-in.nsi
  
SectionEnd

Section "-Shortcuts"
  SetOutPath "$INSTDIR\bin"
  CreateDirectory "$SMPROGRAMS\Melampig"
  CreateShortCut "$DESKTOP\Melampig.lnk" "$INSTDIR\bin\melampig.exe" "$INSTDIR\bin" "$INSTDIR\manager.ico"  
  CreateShortCut "$SMPROGRAMS\Melampig\Melampig.lnk" "$INSTDIR\bin\melampig.exe" "$INSTDIR\bin" "$INSTDIR\manager.ico"  
  CreateShortCut "$SMPROGRAMS\Melampig\uninstall.lnk" "$INSTDIR\uninstall.exe"
  CreateShortCut "$QUICKLAUNCH\Melampig.lnk" "$INSTDIR\bin\melampig.exe" "" "$INSTDIR\manager.ico" 
  WriteRegStr     ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "${PRODUCT_STARTMENU_REGVAL}" "$INSTDIR\manager.ico"
SectionEnd

;--------------------------------
;Installer Functions

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
  WriteRegStr       ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegExpandStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninstall.exe"
  WriteRegStr       ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\manager.ico"
  WriteRegStr       ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr       ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "http://agitum.ru/melampig"
  WriteRegExpandStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "HelpLink" "${PRODUCT_WEB_SITE}"
  WriteRegStr       ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" $(Publisher)
  WriteRegDWORD     ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "NoModify" "1"
  WriteRegDWORD     ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "NoRepair" "1"
FunctionEnd

;--------------------------------
;Descriptions

  LangString DESC_SecManager ${LANG_ENGLISH} "Management program on GR,FS competitions"
  LangString DESC_SecManager ${LANG_RUSSIAN} "Программа проведения соревнования по Вольной,Греко-Римской и Женской борьбе"

  LangString Publisher       ${LANG_ENGLISH} "Alexey Zayats"
  LangString Publisher       ${LANG_RUSSIAN} "Алексей Заяц"

  ;Assign descriptions to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecManager} $(DESC_SecManager)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

 
;--------------------------------
;Uninstaller Section

UninstallText "This will uninstall Melampig. Hit next to continue."
UninstallIcon "${MELAMPIG}\icons\manager.ico"

Section "Uninstall"

  Delete $INSTDIR\bin\libpq.dll
  Delete $INSTDIR\bin\SSLEAY32.dll
  Delete $INSTDIR\bin\LIBEAY32.dll
  Delete $INSTDIR\bin\libintl.dll
  Delete $INSTDIR\bin\libiconv.dll
  
  Delete $INSTDIR\bin\QtCore4.dll
  Delete $INSTDIR\bin\QtGui4.dll
  Delete $INSTDIR\bin\QtNetwork4.dll
  Delete $INSTDIR\bin\QtSql4.dll
  Delete $INSTDIR\bin\QtSvg4.dll
  Delete $INSTDIR\bin\QtXml4.dll
  Delete $INSTDIR\bin\phonon4.dll
  Delete $INSTDIR\bin\QtWebKit4.dll
  
  Delete $INSTDIR\bin\melampig.exe 
  Delete $INSTDIR\bin\orm.dll 
  Delete $INSTDIR\bin\ui.dll
  
  Delete $INSTDIR\bin\sqldrivers\qsqlpsql4.dll
  RMDir $INSTDIR\bin\sqldrivers
  
  Delete $INSTDIR\bin\imageformats\qgif4.dll
  Delete $INSTDIR\bin\imageformats\qico4.dll
  Delete $INSTDIR\bin\imageformats\qjpeg4.dll
  Delete $INSTDIR\bin\imageformats\qmng4.dll
  Delete $INSTDIR\bin\imageformats\qsvg4.dll
  Delete $INSTDIR\bin\imageformats\qtiff4.dll
  RMDir $INSTDIR\bin\imageformats
  
  Delete $INSTDIR\bin\bearer\qgenericbearer4.dll
  Delete $INSTDIR\bin\bearer\qnativewifibearer4.dll
  RMDir $INSTDIR\bin\bearer
    
  Delete $INSTDIR\bin\codecs\qcncodecs4.dll
  Delete $INSTDIR\bin\codecs\qjpcodecs4.dll
  Delete $INSTDIR\bin\codecs\qkrcodecs4.dll
  Delete $INSTDIR\bin\codecs\qtwcodecs4.dll
  RMDir $INSTDIR\bin\codecs
  
  RMDir $INSTDIR\bin
   
  Delete $INSTDIR\lib\competition.dll
  Delete $INSTDIR\lib\reference.dll
  Delete $INSTDIR\lib\reports.dll
  
  RMDir $INSTDIR\lib

  Delete $INSTDIR\share\translations\ru\qt.qm
  Delete $INSTDIR\share\translations\ru\melampig.qm
  Delete $INSTDIR\share\translations\ru\orm.qm
  Delete $INSTDIR\share\translations\ru\ui.qm
  Delete $INSTDIR\share\translations\ru\competition.qm
  Delete $INSTDIR\share\translations\ru\reference.qm
  Delete $INSTDIR\share\translations\ru\reports.qm
  
  Delete $INSTDIR\share\translations\en\melampig.qm
  Delete $INSTDIR\share\translations\en\orm.qm
  Delete $INSTDIR\share\translations\en\ui.qm
  Delete $INSTDIR\share\translations\en\competition.qm
  Delete $INSTDIR\share\translations\en\reference.qm
  Delete $INSTDIR\share\translations\en\reports.qm
  
  RMDir $INSTDIR\share\translations\ru
  RMDir $INSTDIR\share\translations\en
  RMDir $INSTDIR\share\translations
  
  Delete $INSTDIR\share\img\fila-logo.png
  Delete $INSTDIR\share\img\fsw-logo.png
  RMDir "$INSTDIR\share\img"
  
  !include flags-out.nsi
  !include flags2-out.nsi
  !include reports-out.nsi

  RMDir $INSTDIR\share\flags
  RMDir $INSTDIR\share
 
  Delete $DESKTOP\Melampig.lnk
  Delete $SMPROGRAMS\unmelampig.lnk
  Delete $SMPROGRAMS\Melampig.lnk
  Delete $QUICKLAUNCH\Melampig.lnk
  
  RMDir $SMPROGRAMS\Melampig

  Delete "$INSTDIR\manager.ico"
  Delete "$INSTDIR\uninstall.exe"
  
  RMDir "$INSTDIR"
  
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey /ifempty HKCU "Software\Melampig"

SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd
