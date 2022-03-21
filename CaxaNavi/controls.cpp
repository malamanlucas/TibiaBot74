#include "pch.h"
#include "include.h"
#include "menu.h"
#include "controls.h"

/*
* CWindow
*/

CWindow::CWindow()
{
    this->Name = "invalid";
    this->X = 0;
    this->Y = 0;
    this->W = 0;
    this->H = 0;
}

CWindow::CWindow(std::string Name, int* X, int* Y, int W, int H, int* SelectedTab)
{
    this->Name = Name;
    this->X = X;
    this->Y = Y;
    this->W = W;
    this->H = H;
    this->SelectedTab = SelectedTab;

    this->HandlePositioning();

    Render->Rectangle(*X, *Y, W, H, D3DCOLOR_RGBA(25, 25, 25, 255));

    auto TextWidth = Render->GetTextWidth(this->Name.c_str(), FontList::MainFont);

    Render->Rectangle(*X, *Y + H - 13, W, 13, D3DCOLOR_RGBA(0, 0, 0, 100));
    Render->Outline(*X, *Y + H - 13, W, 13, D3DCOLOR_RGBA(32, 32, 32, 255));

    Render->Text(*X + W - TextWidth, *Y + H - 12, FontList::MainFont, this->Name, D3DCOLOR_RGBA(255, 255, 255, 255), false, true, D3DCOLOR_RGBA(0, 0, 0, 255));
}

void CWindow::AddTab(std::string TabName)
{
    this->Tabs.push_back(TabName);
}

void CWindow::AddGroupbox(std::shared_ptr<CGroupbox> Groupbox)
{
    Groupbox->Visible = *this->SelectedTab == Groupbox->Tab;
    if (Groupbox->Visible)
    {
        Groupbox->X += *this->X;
        Groupbox->Y += *this->Y;
        Groupbox->Run();
    }
}

void CWindow::Finish()
{
    auto TabSize = W / Tabs.size();
    for (int i = 0; i < this->Tabs.size(); i++)
    {
        auto Tab = this->Tabs[i];

        RECT TabPosition[4] = { *X + (i * TabSize), *Y, TabSize, 20 };

        bool Hovering = Menu.IsHoveringOverPoint(Vector2D(TabPosition->left, TabPosition->top), Vector2D(TabPosition->right, TabPosition->bottom));

        D3DCOLOR TabColor = *SelectedTab != i ? Hovering ? D3DCOLOR_RGBA(200, 200, 200, 255) : D3DCOLOR_RGBA(160, 160, 160, 255) : D3DCOLOR_RGBA(255, 255, 255, 255);

        if (Hovering && Menu.KeyWasPressed(VK_LBUTTON))
        {
            *SelectedTab = i;
        }

        Render->Rectangle(TabPosition->left, TabPosition->top, TabPosition->right, TabPosition->bottom, *SelectedTab != i ? D3DCOLOR_RGBA(15, 15, 15, 255) : D3DCOLOR_RGBA(25, 25, 25, 255));

        if (*SelectedTab != i)
            Render->Outline(TabPosition->left, TabPosition->top, TabPosition->right, TabPosition->bottom, D3DCOLOR_RGBA(32, 32, 32, 255));

        //if(*SelectedTab == i )
            //Render->Gradient( TabPosition->left, TabPosition->top, TabPosition->right, 2, D3DCOLOR_RGBA( 231, 117, 140, 255 ), D3DCOLOR_RGBA( 191, 77, 100, 255 ), false );
        //else
            //Render->Rectangle( TabPosition->left, TabPosition->top - 1, TabPosition->right, 2, D3DCOLOR_RGBA( 40, 40, 40, 255 ));

        Render->Text(TabPosition->left + (TabPosition->right / 2), TabPosition->top + 3, FontList::MainFont, Tab, *SelectedTab == i ? D3DCOLOR_RGBA(231, 117, 140, 255) : Hovering ? D3DCOLOR_RGBA(200, 200, 200, 255) : D3DCOLOR_RGBA(160, 160, 160, 255), true, true, D3DCOLOR_RGBA(0, 0, 0, 255));
    }

    Render->Outline(*X, *Y, W, H, D3DCOLOR_RGBA(32, 32, 32, 255));
}

void CWindow::HandlePositioning()
{
    static Vector2D DragOffset;
    static bool ShouldBeDragging = false;
    bool Clicked = false;

    if (GetAsyncKeyState(VK_LBUTTON))
        Clicked = true;

    if (ShouldBeDragging && !Clicked)
        ShouldBeDragging = false;

    if (ShouldBeDragging && Clicked)
    {
        *this->X = Menu.MousePosition.x - DragOffset.x;
        *this->Y = Menu.MousePosition.y - DragOffset.y;
    }

    if (Menu.IsHoveringOverPoint(Vector2D(*X, *Y), Vector2D(W, 20)))
    {
        ShouldBeDragging = true;
        DragOffset.x = Menu.MousePosition.x - *X;
        DragOffset.y = Menu.MousePosition.y - *Y;
    }
}

/*
* CGroupbox
*/

CGroupbox::CGroupbox()
{
    this->Name = "invalid";
    this->X = 0;
    this->Y = 0;
    this->W = 0;
    this->H = 0;
    this->Tab = 0;
}

CGroupbox::CGroupbox(std::string Name, int X, int Y, int W, int H, int Tab)
{
    this->Name = Name;
    this->X = X;
    this->Y = Y;
    this->W = W;
    this->H = H;
    this->Tab = Tab;
    this->ControlOffsetX = 12;
    this->ControlOffsetY = 10;
}

void CGroupbox::Run()
{
    Render->Rectangle(X, Y, W, H, D3DCOLOR_RGBA(15, 15, 15, 255));
    Render->Outline(X, Y, W, H, D3DCOLOR_RGBA(30, 30, 30, 255));
    Render->Text(X + 5, Y - 5, FontList::MainFont, this->Name, D3DCOLOR_RGBA(255, 255, 255, 255), false, true, D3DCOLOR_RGBA(0, 0, 0, 255));
}

void CGroupbox::AddControl(std::shared_ptr<CCheckbox> Checkbox)
{
    if (this->Visible)
    {
        Checkbox->X = X + this->ControlOffsetX;
        Checkbox->Y = Y + this->ControlOffsetY;
        Checkbox->Run();
        this->ControlOffsetY += 15;
        MenuInformation::ControlIndex++;
    }
}

void CGroupbox::AddControl(std::shared_ptr<CSlider> Slider)
{
    if (this->Visible)
    {
        Slider->X = X + this->ControlOffsetX;
        Slider->Y = Y + this->ControlOffsetY;
        Slider->Run();
        this->ControlOffsetY += 15;
        MenuInformation::ControlIndex++;
    }
}

void CGroupbox::AddControl(std::shared_ptr<CButton> Button)
{
    if (this->Visible)
    {
        Button->X = X + this->ControlOffsetX;
        Button->Y = Y + this->ControlOffsetY;
        Button->Run();
        this->ControlOffsetY += 21;
        MenuInformation::ControlIndex++;
    }
}

void CGroupbox::AddControl(std::shared_ptr<CCombobox> Combobox)
{
    if (this->Visible)
    {
        Combobox->X = X + this->ControlOffsetX;
        Combobox->Y = Y + this->ControlOffsetY;
        Combobox->Run();
        this->ControlOffsetY += 15;
        MenuInformation::ControlIndex++;
    }
}

/*
* CCheckbox
*/

CCheckbox::CCheckbox()
{
    this->Name = "invalid";
    this->Value = false;
}

CCheckbox::CCheckbox(std::string Name, bool* Value)
{
    this->Name = Name;
    this->Value = Value;
}

void CCheckbox::Run()
{
    auto TextSize = Render->GetTextWidth(this->Name.c_str(), FontList::MainFont);

    bool Hovering = Menu.IsHoveringOverPoint(Vector2D(this->X, this->Y), Vector2D(9 + 5 + TextSize, 8));
    if (Hovering && Menu.KeyWasPressed(VK_LBUTTON))
        *this->Value = !*this->Value;

    Render->Gradient(this->X, this->Y, 9, 9, *this->Value ? D3DCOLOR_RGBA(231, 117, 140, 255) : Hovering ? D3DCOLOR_RGBA(45, 45, 45, 255) : D3DCOLOR_RGBA(25, 25, 25, 255), *this->Value ? D3DCOLOR_RGBA(211, 97, 120, 255) : Hovering ? D3DCOLOR_RGBA(65, 65, 65, 255) : D3DCOLOR_RGBA(45, 45, 45, 255));
    Render->Outline(this->X, this->Y, 9, 9, D3DCOLOR_RGBA(0, 0, 0, 255));

    Render->Text(this->X + 15, this->Y - 2, FontList::MainFont, this->Name, D3DCOLOR_RGBA(255, 255, 255, 255), false, true, D3DCOLOR_RGBA(0, 0, 0, 255));
}

/*
* CSlider
*/

CSlider::CSlider()
{
    this->Name = "invalid";
    this->Value = 0;
    this->FloatSlider = false;
}

CSlider::CSlider(std::string Name, float* Value, float Minimum, float Maximum, bool FloatSlider)
{
    this->Name = Name;
    this->Value = Value;
    this->FloatSlider = FloatSlider;
    this->Minimum = Minimum;
    this->Maximum = Maximum;
}

void CSlider::Run()
{
    static int LastSelectedControlIndex = -1;
    static bool ShouldAffectValue = false;

    auto SliderWidth = 100;

    bool Hovering = Menu.IsHoveringOverPoint(Vector2D(this->X, this->Y), Vector2D(SliderWidth, 9));

    if (Hovering && GetAsyncKeyState(VK_LBUTTON))
        LastSelectedControlIndex = MenuInformation::ControlIndex;

    ShouldAffectValue = GetAsyncKeyState(VK_LBUTTON) && MenuInformation::ControlIndex == LastSelectedControlIndex;

    if (!GetAsyncKeyState(VK_LBUTTON))
        LastSelectedControlIndex = -1;

    if (ShouldAffectValue)
    {
        *this->Value = this->Minimum + (this->Maximum - this->Minimum) * (Menu.MousePosition.x - (this->X)) / (SliderWidth);

        if (*this->Value > this->Maximum)
            *this->Value = this->Maximum;

        if (*this->Value < this->Minimum)
            *this->Value = this->Minimum;
    }

    char Buffer[1024];

    if (this->FloatSlider)
        sprintf_s(Buffer, "%.2f", *this->Value);
    else
        sprintf_s(Buffer, "%i", (int)*this->Value);

    auto Equation = ((*this->Value - this->Minimum) / (this->Maximum - this->Minimum) * (SliderWidth));

    Render->Gradient(this->X, this->Y, SliderWidth, 9, Hovering || ShouldAffectValue ? D3DCOLOR_RGBA(45, 45, 45, 255) : D3DCOLOR_RGBA(25, 25, 25, 255), Hovering || ShouldAffectValue ? D3DCOLOR_RGBA(65, 65, 65, 255) : D3DCOLOR_RGBA(45, 45, 45, 255));
    Render->Gradient(this->X, this->Y, Equation, 9, D3DCOLOR_RGBA(231, 117, 140, 255), D3DCOLOR_RGBA(211, 97, 120, 255));
    Render->Outline(this->X, this->Y, SliderWidth, 9, D3DCOLOR_RGBA(0, 0, 0, 255));

    Render->Text(this->X + SliderWidth + 3, this->Y - 1, FontList::MainFont, this->Name, D3DCOLOR_RGBA(255, 255, 255, 255), false, true, D3DCOLOR_RGBA(0, 0, 0, 255));
    Render->Text(this->X + (SliderWidth / 2), this->Y - 2, FontList::MainFont, Buffer, D3DCOLOR_RGBA(255, 255, 255, 255), true, true, D3DCOLOR_RGBA(0, 0, 0, 255));
}

/*
* CButton
*/

CButton::CButton()
{
    this->Name = "invalid";
    this->Function = NULL;
}

CButton::CButton(std::string Name, ButtonFn Function)
{
    this->Name = Name;
    this->Function = Function;
}

void CButton::Run()
{
    if (!Function)
        return;

    bool Hovering = Menu.IsHoveringOverPoint(Vector2D(this->X, this->Y), Vector2D(100, 15));

    bool Active = Hovering && GetAsyncKeyState(VK_LBUTTON);

    static bool DoOnce;

    if (Active && Menu.KeyWasPressed(VK_LBUTTON) && !DoOnce)
    {
        this->Function();
        DoOnce = true;
    }

    if (!GetAsyncKeyState(VK_LBUTTON))
        DoOnce = false;

    Render->Gradient(this->X, this->Y, 100, 15, Active ? D3DCOLOR_RGBA(45, 45, 45, 255) : D3DCOLOR_RGBA(25, 25, 25, 255), Active ? D3DCOLOR_RGBA(65, 65, 65, 255) : D3DCOLOR_RGBA(45, 45, 45, 255));
    Render->Outline(this->X, this->Y, 100, 15, D3DCOLOR_RGBA(0, 0, 0, 255));
    Render->Text(this->X + (100 / 2), this->Y + 2, FontList::MainFont, this->Name, D3DCOLOR_RGBA(255, 255, 255, 255), true, true, D3DCOLOR_RGBA(0, 0, 0, 255));
}

/*
* CCombobox
*/

CCombobox::CCombobox()
{
    this->Name = "invalid";
    this->Value = 0;
    this->Items = { "" };
}

CCombobox::CCombobox(std::string Name, int* Value, std::vector<std::string> Items)
{
    this->Name = Name;
    this->Value = Value;
    this->Items = Items;
}

void CCombobox::Run()
{
    static int LastHoverIndex = -1;
    static bool ShouldBeOpened = false;

    bool HoveringLeft = Menu.IsHoveringOverPoint(Vector2D(this->X, this->Y), Vector2D(8, 9));
    bool HoveringRight = Menu.IsHoveringOverPoint(Vector2D(this->X + 112, this->Y), Vector2D(8, 9));

    if (*this->Value > 0 && HoveringLeft && Menu.KeyWasPressed(VK_LBUTTON))
        *this->Value -= 1;

    if (*this->Value < this->Items.size() - 1 && HoveringRight && Menu.KeyWasPressed(VK_LBUTTON))
        *this->Value += 1;

    Render->Gradient(this->X + 10, this->Y, 100, 9, D3DCOLOR_RGBA(25, 25, 25, 255), D3DCOLOR_RGBA(45, 45, 45, 255));
    Render->Outline(this->X + 10, this->Y, 100, 9, D3DCOLOR_RGBA(0, 0, 0, 255));
    Render->Text(this->X + 10 + (100 / 2), this->Y - 2, FontList::MainFont, this->Items[*this->Value], D3DCOLOR_RGBA(255, 255, 255, 255), true, true, D3DCOLOR_RGBA(0, 0, 0, 255));
    Render->Text(this->X + 20 + 100 + 3, this->Y - 1, FontList::MainFont, this->Name, D3DCOLOR_RGBA(255, 255, 255, 255), false, true, D3DCOLOR_RGBA(0, 0, 0, 255));


    Render->Gradient(this->X + 112, this->Y, 8, 9, HoveringRight && (*this->Value < this->Items.size() - 1) ? D3DCOLOR_RGBA(45, 45, 45, 255) : D3DCOLOR_RGBA(25, 25, 25, 255), HoveringRight && (*this->Value < this->Items.size() - 1) ? D3DCOLOR_RGBA(65, 65, 65, 255) : D3DCOLOR_RGBA(45, 45, 45, 255));
    Render->Outline(this->X + 112, this->Y, 8, 9, D3DCOLOR_RGBA(0, 0, 0, 255));
    Render->Text(this->X + 112 + (8 / 2), this->Y - 2, FontList::MainFont, "+", (*this->Value < this->Items.size() - 1) ? D3DCOLOR_RGBA(255, 255, 255, 255) : D3DCOLOR_RGBA(125, 125, 125, 255), true, true, D3DCOLOR_RGBA(0, 0, 0, 255));

    Render->Gradient(this->X, this->Y, 8, 9, HoveringLeft && *this->Value > 0 ? D3DCOLOR_RGBA(45, 45, 45, 255) : D3DCOLOR_RGBA(25, 25, 25, 255), HoveringLeft && *this->Value > 0 ? D3DCOLOR_RGBA(65, 65, 65, 255) : D3DCOLOR_RGBA(45, 45, 45, 255));
    Render->Outline(this->X, this->Y, 8, 9, D3DCOLOR_RGBA(0, 0, 0, 255));
    Render->Text(this->X + (8 / 2), this->Y - 1, FontList::MainFont, "-", *this->Value > 0 ? D3DCOLOR_RGBA(255, 255, 255, 255) : D3DCOLOR_RGBA(125, 125, 125, 255), true, true, D3DCOLOR_RGBA(0, 0, 0, 255));
}