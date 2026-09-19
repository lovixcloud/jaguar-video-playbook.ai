!define PRODUCT_NAME "Jaguar Studio"
!define PRODUCT_VERSION "1.0.0"
!define PRODUCT_PUBLISHER "Jaguar"
!define PRODUCT_WEB_SITE "https://jaguarvideo.app"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\Jaguar.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "JaguarSetup.exe"
InstallDir "$PROGRAMFILES64\Jaguar Studio"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifNewer
  File "build\Jaguar.exe"
  File "README.md"
  File "LICENSE"

  CreateDirectory "$SMPROGRAMS\Jaguar Studio"
  CreateShortCut "$SMPROGRAMS\Jaguar Studio\Jaguar Studio.lnk" "$INSTDIR\Jaguar.exe"
  CreateShortCut "$DESKTOP\Jaguar Studio.lnk" "$INSTDIR\Jaguar.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\Jaguar.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
SectionEnd

Section "Uninstall"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\Jaguar.exe"
  Delete "$INSTDIR\README.md"
  Delete "$INSTDIR\LICENSE"
  Delete "$SMPROGRAMS\Jaguar Studio\Jaguar Studio.lnk"
  Delete "$DESKTOP\Jaguar Studio.lnk"

  RMDir "$SMPROGRAMS\Jaguar Studio"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
SectionEnd
