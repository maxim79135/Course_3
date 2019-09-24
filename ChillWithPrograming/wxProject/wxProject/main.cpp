#include "wx/wx.h"

class cApp : public wxApp {
public:
    virtual bool OnInit() {
        wxFrame *m_frame1 = new wxFrame(nullptr, wxID_ANY, "Hello");
        m_frame1->Show();
    }
};

