// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryCharWidget.h"
#include "Cube/DebugMacros.h"
#include "Cube/Factory/InventoryManager.h"
#include "Cube/HumanController.h"


void UInventoryCharWidget::NativeConstruct()
{
	Super::NativeConstruct();
	DEBUG_CHECK_RETURN("IventoryCharWidget", "objects classes", Weapon_Slot && Armor_Slot && Helmet_Slot && SlotWidgetClass);
}

void UInventoryCharWidget::RefreshInventory()
{
	DEBUG_CHECK_RETURN("IventoryCharWidget", "objects classes", Weapon_Slot && Armor_Slot && Helmet_Slot && SlotWidgetClass);

	Weapon_Slot->SetItem(InventoryManager->GetItem (EItemsInInventoryID::weapon).Object, -1);
	Weapon_Slot->SetConfig(SlotWidgetClass, InventoryManager, EItemsInInventoryID::weapon);

	Armor_Slot->SetItem(InventoryManager->GetItem(EItemsInInventoryID::armor).Object, -1);
	Armor_Slot->SetConfig(SlotWidgetClass, InventoryManager, EItemsInInventoryID::armor);

	Helmet_Slot->SetItem(InventoryManager->GetItem(EItemsInInventoryID::helmet).Object, -1);
	Helmet_Slot->SetConfig(SlotWidgetClass, InventoryManager, EItemsInInventoryID::helmet);
}

void UInventoryCharWidget::SetInventoryManager(UInventoryManager* NewInventoryManager)
{
	DEBUG_CHECK_RETURN("IventoryWidget", "IventoryManager", NewInventoryManager)
	InventoryManager = NewInventoryManager;
	InventoryManager->OnItemsChanged.AddDynamic(this, &UInventoryCharWidget::RefreshInventory);
}
