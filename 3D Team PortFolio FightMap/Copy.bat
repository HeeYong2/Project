

// xcopy 옵션 .복사 원본경로 .복사받아야할 대상경로
xcopy /y .\Engine\Export_Function\*.*				.\Reference\Headers\
xcopy /y .\Engine\Export_Function\System\*.*		.\Reference\Headers\
xcopy /y .\Engine\Export_Function\Resource\*.*		.\Reference\Headers\
xcopy /y .\Engine\Export_Function\Utility\*.*		.\Reference\Headers\
xcopy /y .\Engine\Headers\*.*						.\Reference\Headers\

xcopy /y .\Engine\System\Codes\*.h					.\Reference\Headers\
xcopy /y .\Engine\System\bin\System.lib				.\Reference\Librarys\

xcopy /y .\Engine\System\bin\System.dll				.\Tool\Bin\

xcopy /y .\Engine\Utility\Codes\*.h					.\Reference\Headers\
xcopy /y .\Engine\Utility\Codes\*.inl				.\Reference\Headers\
xcopy /y .\Engine\Utility\bin\Utility.lib			.\Reference\Librarys\
xcopy /y .\Engine\Utility\bin\Utility.dll			.\Tool\Bin\

xcopy /y .\Engine\Resource\Codes\*.h				.\Reference\Headers\
xcopy /y .\Engine\Resource\Codes\*.inl				.\Reference\Headers\
xcopy /y .\Engine\Resource\bin\Resource.lib			.\Reference\Librarys\
xcopy /y .\Engine\Resource\bin\Resource.dll			.\Tool\Bin\