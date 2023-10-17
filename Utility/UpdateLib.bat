@echo off

xcopy	/y/s	..\Engine\Resources\*.*				..\Engine\Bin\Resources\
xcopy	/y/s	..\Engine\Resources\*.*				..\Client\Bin\Resources\
xcopy	/y/s	..\Engine\Resources\Shader\*.hlsl	..\Resources\Shader\
xcopy	/y/s	..\Engine\Resources\Shader\*.hlsli	..\Resources\Shader\
echo	Resources Complete!

xcopy	/y/s	..\Engine\Public\*.h				..\Reference\Header\
xcopy	/y/s	..\Engine\Public\*.hpp				..\Reference\Header\
echo	Header Complete!

xcopy	/y/s	..\Engine\Public\*.inl				..\Reference\Header\
echo	Inline Complete!

xcopy	/y		..\Engine\Bin\*.lib					..\Reference\Library\
xcopy	/y		..\Engine\ThirdPartyLib\*.lib		..\Reference\Library\
xcopy	/y		..\Reference\Library\*.dll			..\Client\Bin\
echo	Library Complete!

xcopy	/y		..\Engine\Bin\*.dll					..\Client\Bin\
echo	Engine Complete!

echo	Done!
