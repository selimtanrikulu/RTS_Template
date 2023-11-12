// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/StoreNode.h"

StoreNode::StoreNode(FString name)
{
	Name = name;
}

StoreNode::~StoreNode()
{
}

void StoreNode::LogInfo() const
{
	UE_LOG(LogTemp,Display,TEXT("Node : %s"),*Name);
}
