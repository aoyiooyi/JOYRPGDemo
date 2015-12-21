-- Boot mode configuration parameters and data files to load

-- -------------------------------------------------- --
-- ----------------- VIDEO STUFF -------------------- --
-- -------------------------------------------------- --

--bgImage = "./image/boot/bg.png"
--bgLogo = "./image/boot/bgLogo.png"
--sword = "./image/boot/sword.png"
--gameTitle = "./image/boot/gameTitle.png"
cursor = "./image/cursor/cursor.png"
menuButton = "./image/button/menuButton.png"

bgImage = "boot_ui.bg"
bgLogo = "boot_ui.bgLogo"
sword = "boot_ui.sword"
gameTitle = "boot_ui.gameTitle"
--cursor = "boot_ui.cursor"
--menuButton = "./image/button/menuButton.png"

-- -------------------------------------------------- --
-- ----------------- AUDIO STUFF -------------------- --
-- -------------------------------------------------- --
bgMusic = "./mus/background.ogg";
openingMusic = "./mus/opening.ogg";
clickSound = "./snd/menu.wav";



font = "./fnt/font1.fnt"


function NewGame()

	--[����scene
	local newScene = CreateSceneMode();
	--local newScene = SceneMode();
	newScene:Initialize();
	newScene:ChangeLocation("./dat/map/���ٴ�.lua");

	--����ModeManager
	local theManager = ModeManagerCreate();
	theManager:Push(CreateSceneMode());


end
