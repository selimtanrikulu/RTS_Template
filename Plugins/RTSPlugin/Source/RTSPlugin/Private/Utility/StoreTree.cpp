// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/StoreTree.h"


StoreTree::StoreTree(StoreNode* node, StoreTree* parent)
{
	Node = node;
	Parent = parent;
}

StoreTree::~StoreTree()
{
}


StoreTree* StoreTree::GetChildWithName(const FString& Category)
{
	for(StoreTree* Child : Children)
	{
		if(Child->Node->Name == Category)
		{
			return Child;
		}
	}
	return nullptr;
}

TArray<FString> StoreTree::GetTail(TArray<FString> &Path)
{
	TArray<FString> Tail;
	for(int i = 1;i<Path.Num();i++)
	{
		Tail.Add(Path[i]);
	}

	return Tail;
}

void StoreTree::AddCategory(TArray<FString> &Path,  FBuildingData& BuildingData)
{

	Node->ContainingBuildings.Add(BuildingData);

	if(Path.Num() == 0)
	{
		//UE_LOG(LogTemp,Display,TEXT("Path Constructed"));
		return;
	}



	const FString ChildName = Path[0];
	
	//Add Category Uniquely
	StoreTree* Child = GetChildWithName(ChildName);
	
	if(!Child)
	{
		Child = new StoreTree(new StoreNode(ChildName),this);
		Children.Add(Child);
	}

	TArray<FString> Tail = GetTail(Path);


	Child->AddCategory(Tail,BuildingData);
}

void StoreTree::LogInfo()
{
	UE_LOG(LogTemp,Display,TEXT("Tree Info : "));
	TArray<TArray<FString>> AllPaths = GetAllPaths();
	for(TArray<FString> &Path : AllPaths)
	{
		UE_LOG(LogTemp,Display,TEXT("Path : "));

		for(FString Category : Path)
		{
			UE_LOG(LogTemp,Display,TEXT("%s"),*Category);
		}
	}
}


TArray<TArray<FString>> StoreTree::GetAllPaths()
{
	if(Children.Num() == 0)
	{
		TArray<TArray<FString>> Result;
		TArray<FString> Single;
		Single.Add(Node->Name);
		Result.Add(Single);
		return Result;
	}

	
	TArray<TArray<FString>> AllPaths;


	const FString Category = Node->Name;

	for(StoreTree* Child : Children)
	{
		TArray<TArray<FString>> ChildAllPaths = Child->GetAllPaths();

		for(TArray<FString> &ChildPath : ChildAllPaths)
		{
			ChildPath.Insert(Category,0);
		}

		AllPaths.Append(ChildAllPaths);
	}

	return AllPaths;
}

TArray<FBuildingData> StoreTree::GetBuildingsByPath(TArray<FString>& Path)
{
	if(Path.Num() == 0)
	{
		return Node->ContainingBuildings;
	}
	
	StoreTree* Child = GetChildWithName(Path[0]);

	if(!Child)
	{
		TArray<FBuildingData> Empty;
		return Empty;
	}
	
	TArray<FString> Tail = GetTail(Path);

	
	return Child->GetBuildingsByPath(Tail);
}

TArray<FString> StoreTree::GetChildrenNames()
{
	TArray<FString> ChildrenNames;

	for(const StoreTree* Child : Children)
	{
		ChildrenNames.Add(Child->Node->Name);
	}

	return ChildrenNames;
}

StoreTree* StoreTree::GetStoreTreeByPath(TArray<FString> &Path)
{
	if(Path.Num() == 0)
	{
		return this;
	}

	const FString ChildName = Path[0];
	StoreTree* ChildTree = GetChildWithName(ChildName);
	
	if(!ChildTree)
	{
		UE_LOG(LogTemp,Warning,TEXT("Path is not valid"));
		return this;
	}

	TArray<FString> Tail = GetTail(Path);
	return ChildTree->GetStoreTreeByPath(Tail);
}