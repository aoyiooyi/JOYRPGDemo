cursorImg = "./image/cursor/cursor2.png";
cursorX = 0;
cursorY = 0;

PortalImg = "./image/map/portal.png"
frams =   4
fps =     5
tx =      0
ty =      0
w =       128
h =       128


function Initialize(scene)

	--读取和设置鼠标
	scene:SetCursor(cursorImg, cursorX, cursorY);

	--读取传送点资源
	scene:LoadPortalAnm(PortalImg, frams, fps, tx, ty, w, h);

end
