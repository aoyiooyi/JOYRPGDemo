

dlgImage = "./image/gui/dialogBGI.png"
dlgImageWidth = 480
dlgImageHeight = 166

NPCImage = "./image/character/shenxian1/Head.png"
NPCImageWidth = 148
NPCImageHeight = 192


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
DialogPages[1].choicesNum = 1
for i = 1, DialogPages[1].choicesNum, 1 do
    DialogPages[1].choices[i] = {}
end

DialogPages[1].sentences[1].words = "Is No One Interested inHistory?"
DialogPages[1].sentences[1].align = 0
DialogPages[1].sentences[1].color = -1

--DialogPages[1].sentences[2].words = "welcome"
--DialogPages[1].sentences[2].align = 1
--DialogPages[1].sentences[2].color = -1

DialogPages[1].choices[1].words = "Onwards?"
DialogPages[1].choices[1].color = -3661021
DialogPages[1].choices[1].funcName = "Onwards"

--DialogPages[1].choices[2].words = "Let me think!!"
--DialogPages[1].choices[2].color = -3661021
--DialogPages[1].choices[2].funcName = "battle"
---3661021


function Onwards(dialog)
	dialog:Clear();
	dialog:AddSentence("Oh it's the TARDIS.", 0, -1);
	dialog:AddSentence("It's my home.", 1, -1);
	dialog:AddChoice("haha", -3661021, "CloseDialog");
end

function CloseDialog(dialog)
	local GameGlobalInfo = GetGameGlobalInfo();
	local GUIManager = GameGlobalInfo:GetGUIManager();
	GUIManager:ReleasePanel(dialog);
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


