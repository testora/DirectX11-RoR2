@echo off

xcopy	/y/s	..\Engine\Public\*.h				..\Reference\Header\
xcopy	/y/s	..\Engine\Public\*.hpp				..\Reference\Header\
echo Header Complete!

xcopy	/y/s	..\Engine\Public\*.inl				..\Reference\Header\
echo Inline Complete!

xcopy	/y		..\Engine\Bin\*.lib					..\Reference\Library\
xcopy	/y		..\Engine\ThirdPartyLib\*.lib		..\Reference\Library\
echo Library Complete!

xcopy	/y		..\Engine\Bin\*.dll					..\Client\Bin\
echo Bin Complete!

echo Done!
