

dlgImage = "./image/gui/dialogBGI.png"
dlgImageWidth = 480
dlgImageHeight = 166

NPCImage = "./image/character/snake/Head.png"
NPCImageWidth = 157
NPCImageHeight = 217


----[[
DialogPages = {}

DialogPages.DialogPagesNum = 1
for i = 1, DialogPages.DialogPagesNum, 1 do
	DialogPages[i] = {}
end


DialogPages[1].sentences = {}
DialogPages[1].sentencesNum = 1
for i = 1, DialogPages[1].sentencesNum, 1 do
    DialogPages[1].sentences[i] = {}
end

DialogPages[1].choices = {}
DialogPages[1].choicesNum = 2
for i = 1, DialogPages[1].choicesNum, 1 do
    DialogPages[1].choices[i] = {}
end

DialogPages[1].sentences[1].words = "Hero,die!!"
DialogPages[1].sentences[1].align = 0
DialogPages[1].sentences[1].color = -1


DialogPages[1].choices[1].words = "shut up!!"
DialogPages[1].choices[1].color = -3661021
DialogPages[1].choices[1].funcName = "battle"

DialogPages[1].choices[2].words = "run away"
DialogPages[1].choices[2].color = -3661021
DialogPages[1].choices[2].funcName = "run"
---3661021

function run(dialog)
	local GameGlobalInfo = GetGameGlobalInfo();
	local GUIManager = GameGlobalInfo:GetGUIManager();
	GUIManager:ReleasePanel(dialog);
end

function battle(dialog)
	local GameGlobalInfo = GetGameGlobalInfo();
	local GUIManager = GameGlobalInfo:GetGUIManager();
	GUIManager:ReleasePanel(dialog);

	StartBattle();
end



function FillDialogText(dialog)

	--[[
	for i = 1, DialogPages.DialogPagesNum, 1 do

		-- read the sentences
		for j = 1, DialogPages[i].sentencesNum, 1 do

			dialog:AddSentence(DialogPages[i].sentences[j].words, DialogPages[i].sentences[j].align, DialogPages[i].sentences[j].color);

		end

		-- read the choices
		for j = 1, DialogPages[i].choicesNum, 1 do

			dialog:AddChoice(DialogPages[i].choices[j].words, DialogPages[i].choices[j].color, DialogPages[i].choices[j].funcName);

		end


	end
	--]]

end


