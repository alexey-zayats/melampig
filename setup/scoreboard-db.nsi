;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General

  ;Name and file
  Name "ScoreBoard"
  OutFile "ScoreBoard.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\ScoreBoard"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\ScoreBoard" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Language Selection Dialog Settings

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKCU" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\ScoreBoard" 
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

!define PRODUCT_NAME "ScoreBoard"
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
  
  SetOutPath "$INSTDIR\bin\phonon_backend"
  File ${QT}\plugins\phonon_backend\phonon_ds94.dll
  
  SetOutPath "$INSTDIR"
  File ${MELAMPIG}\icons\manager.ico 
  
  WriteUninstaller "$INSTDIR\uninstall.exe"
SectionEnd

Section "ScoreBoard" SecManager
  SetOutPath "$INSTDIR\bin"
  File ${MELAMPIG}\bin\scoreboard-db.exe 
  File ${MELAMPIG}\bin\orm.dll 
  File ${MELAMPIG}\bin\ui.dll
  SetOutPath "$INSTDIR\share\sound"
  File ${MELAMPIG}\share\sound\001.wav 
  File ${MELAMPIG}\share\sound\002.wav
  File ${MELAMPIG}\share\sound\003.wav

  SetOutPath "$INSTDIR\share\translations"
  File ${MELAMPIG}\share\translations\ru\qt.qm
  File ${MELAMPIG}\share\translations\ru\orm.qm
  File ${MELAMPIG}\share\translations\ru\ui.qm

SectionEnd

Section "-Shortcuts"
  SetOutPath "$INSTDIR\bin"
  CreateDirectory "$SMPROGRAMS\ScoreBoard"
  CreateShortCut "$DESKTOP\ScoreBoard.lnk" "$INSTDIR\bin\scoreboard-db.exe" "$INSTDIR\bin" "$INSTDIR\manager.ico"  
  CreateShortCut "$SMPROGRAMS\ScoreBoard\ScoreBoard.lnk" "$INSTDIR\bin\scoreboard-db.exe" "$INSTDIR\bin" "$INSTDIR\manager.ico"  
  CreateShortCut "$SMPROGRAMS\ScoreBoard\uninstall.lnk" "$INSTDIR\uninstall.exe"
  CreateShortCut "$QUICKLAUNCH\ScoreBoard.lnk" "$INSTDIR\bin\scoreboard-db.exe" "" "$INSTDIR\manager.ico" 
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

  LangString DESC_SecManager ${LANG_ENGLISH} "Display program on GR,FS competitions"
  LangString DESC_SecManager ${LANG_RUSSIAN} "Программа отображения ходы поединка соревнования по Вольной,Греко-Римской и Женской борьбе"

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

  Delete $INSTDIR\bin\scoreboard-db.exe 
  Delete $INSTDIR\bin\orm.dll

  Delete $INSTDIR\bin\sqldrivers\qsqlpsql4.dll
  Delete $INSTDIR\bin\imageformats\qgif4.dll
  Delete $INSTDIR\bin\imageformats\qico4.dll
  Delete $INSTDIR\bin\imageformats\qjpeg4.dll
  Delete $INSTDIR\bin\imageformats\qmng4.dll
  Delete $INSTDIR\bin\imageformats\qsvg4.dll
  Delete $INSTDIR\bin\imageformats\qtiff4.dll

  Delete $INSTDIR\bin\phonon_backend\phonon_ds94.dll
  RMDir $INSTDIR\bin\phonon_backend
 
  Delete $INSTDIR\share\translations\orm_ru.qm 
  Delete $INSTDIR\share\translations\ui_ru.qm 
  Delete $INSTDIR\share\translations\qt_ru.qm
  RMDir $INSTDIR\share\translations
  
  Delete $INSTDIR\share\sound\001.wav 
  Delete $INSTDIR\share\sound\002.wav
  Delete $INSTDIR\share\sound\003.wav
  RMDir $INSTDIR\share\sound
  
  Delete $DESKTOP\ScoreBoard.lnk
  Delete $SMPROGRAMS\uninstall.lnk
  Delete $SMPROGRAMS\ScoreBoard.lnk
  Delete $QUICKLAUNCH\ScoreBoard.lnk

  RMDir $INSTDIR\bin\phonon_backend
  RMDir $INSTDIR\bin\imageformats
  RMDir $INSTDIR\bin\sqldrivers
  RMDir $INSTDIR\bin
  RMDir $INSTDIR\lib
  RMDir $INSTDIR\share

  RMDir $SMPROGRAMS\ScoreBoard

  Delete "$INSTDIR\manager.ico"
  Delete "$INSTDIR\uninstall.exe"
  
  RMDir "$INSTDIR"
  
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey /ifempty HKCU "Software\ScoreBoard"

SectionEnd

;--------------------------------
;Uninstaller Functions

Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd
