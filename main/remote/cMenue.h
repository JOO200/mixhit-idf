#ifndef _CMENUE_H_
#define _CMENUE_H_
#include "cMenueItem.h"

class cMenue
{
public:
	cMenue();
	bool Button_Up();			// Ruft die Funktion UP des angezeiten MenuePunktes auf.
	bool Button_Down();		// Ruft die Funktion DOWN des angezeiten MenuePunktes auf.
	bool Button_Back();		// Ruft die Funktion BACK des angezeiten MenuePunktes auf.
	bool Button_Ok();			// Ruft die Funktion OK des angezeiten MenuePunktes auf.
	void showMenue();	// Zeigt das ausgewaehlte Menue auf dem OLED Display an.
	bool selectMenueItem(int* pPath, int pNumberOfIndices);				// Waehlt das gewuenschte Menue aus und schreibt die Adresse in "mSelectedMenueItem"
	bool addSubMenueItemToSelectedMenueItem(cMenueItem* pMenueItem);	// Fuegt ein Menuepunkt als SubMenue des "mSelectedMenueItem" hinzu.
	cMenueItem* mMenueItem;												// Einstiegspunkt des Menues.
	cMenueItem* mSelectedMenueItem;										// Aktuell angezeigter Menuepunkt - Dies ist NICHT der ausgewaehlte Menupunkt "> ..." (dies wäre der SelectedIndex des angezeigten Menuepunkts).
};
extern cMenue gMenue;
#endif
