#include "FactorySubsystem.h"
#include "Cube/DebugMacros.h"

void UFactorySubsystem::Tick(float DeltaTime)
{
	
	ParallelFor(MachinesList.Num(), [&](int32 Index)
		{
			
			FMachine& TecMachine = MachinesList[Index];

			DEBUG_CHECK("UFactorySubsystem::Tick", TecMachine.RecipeID.IsValid());
			FRecipe& TecRecipe = RecipesList[RecipeLookupMap[TecMachine.RecipeID]];
			

			if (TecMachine.Progress != -1)
			{
				TecMachine.Progress++;
				if (TecMachine.Progress >= TecRecipe.TicksToCreate)
				{
					bool OK = 1;
					if (TecRecipe.Outputs.Num() > TecMachine.Outputs.Num())
						OK = 0;
					else
						for (int i = 0; i < TecRecipe.Outputs.Num(); i++)
						{
							if (TecMachine.Outputs[i].ID != TecRecipe.Outputs[i].ID ||
								TecMachine.Outputs[i].Num + TecRecipe.Outputs[i].Num > 100)
							{
								OK = 0;
								break;
							}
						}


					if (OK)
					{
						TecMachine.Progress = -1;
						for (int i = 0; i < TecRecipe.Outputs.Num(); i++)
						{
							TecMachine.Outputs[i].Num += TecRecipe.Outputs[i].Num;
						}
					}
				}
				
			}


			if (TecMachine.Progress == -1)
			{
				bool OK = 1;
				if (TecRecipe.Inputs.Num() > TecMachine.Inputs.Num())
					OK = 0;
				else
					for (int i = 0; i < TecRecipe.Inputs.Num(); i++)
					{
						if (TecMachine.Inputs[i].ID != TecRecipe.Inputs[i].ID ||
							TecMachine.Inputs[i].Num < TecRecipe.Inputs[i].Num)
						{
							OK = 0;
							break;
						}
					}


				if (OK)
				{
					TecMachine.Progress = 0;
					for (int i = 0; i < TecRecipe.Inputs.Num(); i++)
					{						
						TecMachine.Inputs[i].Num -= TecRecipe.Inputs[i].Num;
					}
				}
				

			}

		



		});
		
	return;
}

TStatId UFactorySubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFactoryManagerSubsystem, STATGROUP_Tickables);
}

void UFactorySubsystem::RegisterMachine(FMachine Machine)
{
	MachinesList.Add(Machine);
	MachineLookupMap.Add(Machine.MachineID, MachinesList.Num() - 1);
}

void UFactorySubsystem::RegisterRecipe(FRecipe Recipe)
{
	RecipesList.Add(Recipe);
	RecipeLookupMap.Add(Recipe.RecipeID, RecipesList.Num() - 1);
	RecipesNamesMap.Add(Recipe.Name, Recipe.RecipeID);
}
