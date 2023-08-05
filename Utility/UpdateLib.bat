@echo off

xcopy	/y/s	..\Engine\Public\*.h				..\Reference\Header\
echo Header Complete!

xcopy	/y		..\Engine\Public\*.inl				..\Reference\Header\
echo Inline Complete!

xcopy	/y		..\Engine\Bin\*.lib					..\Reference\Library\
echo Dynamic Library Complete!

xcopy	/y		..\Engine\ThirdPartyLib\*.lib		..\Reference\Library\
echo Static Library Complete!

xcopy	/y		..\Engine\Bin\*.dll					..\Client\Bin\
echo Bin Complete!

echo Done!
