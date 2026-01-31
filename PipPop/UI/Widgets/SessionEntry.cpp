// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/SessionEntry.h"

void USessionEntry::SetSessionText(const ESessionText TextType, const FText& Text)
{
	switch (TextType)
	{
		case ESessionText::ServerName:
			if (SessionButtons.ServerName) {SessionButtons.ServerName->SetText(Text);}
			break;
		case ESessionText::MapName:
			if (SessionButtons.MapName) {SessionButtons.MapName->SetText(Text);}
			break;
		case ESessionText::CurrentPlayers:
			if (SessionButtons.CurrentPlayers) {SessionButtons.CurrentPlayers->SetText(Text);}
			break;
		case ESessionText::MaxPlayers:
			if (SessionButtons.MaxPlayers) {SessionButtons.MaxPlayers->SetText(Text);}
			break;
		case ESessionText::ModeName:
			if (SessionButtons.ModeName) {SessionButtons.ModeName->SetText(Text);}
			break;
		case ESessionText::Ping:
			if (SessionButtons.Ping) {SessionButtons.Ping->SetText(Text);}
			break;
	}
}

