#ifndef _CMENUEITEM_H_
#define _CMENUEITEM_H_

#include <string>
#include "../mixer/Configuration.h"
#include "../driver/cOLED.h"

class cMenueItem;

typedef void(*OK_Function)(cMenueItem& pSender);

class cMenueItem
{
public:
	cMenueItem();											// Standartkonstruktor
	cMenueItem(std::string pName, OK_Function pOK_Function);		// Konstruktor mit Informationen
	std::string getSubmenueItemName(int pIndex);					// Gibt den Namen des Submenueitems (eine Ebene tiefer) an der angegebenen Stelle aus.
	std::string getNextMenueItemName(int pIndex);				// Gibt den Namen des Menueitems (in der selben Ebene (layer)) an der gegebenen Stelle aus.
	cMenueItem* getSubmenueItem(int pIndex);				// Gibt die Adresse des Submenueitems (eine Ebene tiefer) an der gegebenen Stelle aus.
	cMenueItem* getNextMenueItem(int pIndex);				// Gibt die Adresse des Menueitems (in der selben Ebene (layer)) an der gegebenen Stelle aus.
	void Up();												// Veringert den mSelectedIndex um eins (Menue hoch).
	void Down();											// Erhoeht den mSelectedIndex um eins (Menue runter).
	void Back();											// Waehlt das ParentMenueItem (eine Ebene hoeher) aus.
	void Ok();												// Waehlt ggf. das SubMenueItem an der Stelle mSelectedIndex aus und fuehrt dessen mOK_Funktion aus.
	void drawDisplay();										// Erstellt die Zeilen welche auf dem Display angezeigt werden sollen.
	void addSubMenueItem(cMenueItem* pMenueItem);			// Fuegt ein SubMenueItem (eine Ebene tiefer) hinzu.
	void addMenueItem(cMenueItem* pMenueItem);				// Fuegt ein MenueItem (in der selben Ebene) hinzu.
	cMenueItem* selectMenueItem(int* pPath, int pNumberOfIndices, int pLayer); // Gibt die Adresse des MenueItems / SubMenueItems in der angegebenen Ebene an angegebenen Pfad aus.
	cMenueItem* mNextMenueItem;								// Adresse des direkten nachfolger MenueItems (in der selben Ebene).
	cMenueItem* mSubMenueItems;								// Adresse des ersten SubmenueItems (eine Ebene tiefer).
	cMenueItem* mParent;									// ParentMenueItem (eine Ebene hoeher) (dieses MenueItem ist ein SubMenueItem des ParentMenueItem).
	int mLayer;												// Ebene des MenueItems.
	int mIndex;												// Index (in der selben Ebene) des MenueItems.
	int mSelectedIndex;										// Indes des ausgewaehlten SubMenueItems.
	int mSubMenueItems_Count;								// Anzahl an SubMenueItems.
private:
	OK_Function mOK_Function;								// Funktion die beim klicken auf OK ausgefuehrt wird.
	std::string mName;											// Name des MenueItems.
};

#endif
