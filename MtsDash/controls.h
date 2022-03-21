#pragma once
#include "include.h"
#include "menu.h"
/*
* CWindow
*/

class CGroupbox;

class CWindow : public CMenu
{
public:
    CWindow();
    CWindow(std::string Name, int* X, int* Y, int W, int H, int* SelectedTab = 0);
    void AddTab(std::string TabName);
    void AddGroupbox(std::shared_ptr<CGroupbox> Groupbox);
    void Finish();
    void HandlePositioning();
    int* X; int* Y; int W; int H;
    std::string Name;
    int* SelectedTab;
    std::vector<std::string> Tabs;
private:

};

/*
* CGroupbox
*/

class CCheckbox;
class CSlider;
class CButton;
class CCombobox;

class CGroupbox : public CWindow
{
public:
    CGroupbox();
    CGroupbox(std::string Name, int X, int Y, int W, int H, int Tab = 0);
    void Run();
    void AddControl(std::shared_ptr<CCheckbox> Checkbox);
    void AddControl(std::shared_ptr<CSlider> Slider);
    void AddControl(std::shared_ptr<CButton> Button);
    void AddControl(std::shared_ptr<CCombobox> Combobox);
    std::string Name;
    int X, Y, W, H;
    int Tab;
    int ControlOffsetX, ControlOffsetY;

    bool Visible;
};

/*
* CCheckbox
*/

class CCheckbox : public CGroupbox
{
public:
    CCheckbox();
    CCheckbox(std::string Name, bool* Value);
    void Run();

    int X;
    int Y;
    std::string Name;
    bool* Value;
};

/*
* CSlider
*/

class CSlider : public CGroupbox
{
public:
    CSlider();
    CSlider(std::string Name, float* Value, float Minimum = 0, float Maximum = 100, bool FloatSlider = false);
    void Run();

    int X;
    int Y;
    std::string Name;
    float* Value;
    bool FloatSlider;
    float Minimum;
    float Maximum;
};

/*
* CButton
*/

class CButton : public CGroupbox
{
public:
    typedef void(*ButtonFn)(void);

    CButton();
    CButton(std::string Name, ButtonFn Function);
    void Run();

    std::string Name;
    ButtonFn Function;
};

/*
* CCombobox
*/

class CCombobox : public CGroupbox
{
public:
    CCombobox();
    CCombobox(std::string Name, int* Value, std::vector<std::string> Items);
    void Run();

    int X;
    int Y;
    std::string Name;
    int* Value;
    std::vector<std::string> Items;
};