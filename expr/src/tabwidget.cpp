#include "tab.h"
#include "tabwidget.h"
#include "webview.h"

#include <QTabBar>

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget (parent)
{
    m_newTabButton = new QToolButton;

    m_newTabButton->setAutoRaise(true);
    m_newTabButton->setIcon(QIcon::fromTheme(QStringLiteral("list-add")));

    QTabBar *tabBar = this->tabBar();
    tabBar->setDocumentMode(true);
    tabBar->setExpanding(true);
    tabBar->setMovable(true);
    tabBar->setTabsClosable(true);
    tabBar->setUsesScrollButtons(true);

    setContentsMargins(0, 0, 0, 0);

    setCornerWidget(m_newTabButton);

    addTab(new Tab, QStringLiteral("New Tab"));

    connect(m_newTabButton, &QToolButton::clicked, this, [this] { addTab(new Tab, QStringLiteral("New Tab")); });
    connect(this, &TabWidget::currentChanged, this, [this] (int index) {
        Tab *tab = dynamic_cast<Tab *>(widget(index));
        if (!tab) {
            return ;
        }

        emit urlChanged(tab->webView()->url());
    });
    connect(tabBar, &QTabBar::tabCloseRequested, this, [this] (int index) {
        Tab *tab = dynamic_cast<Tab *>(widget(index));
        if (!tab) {
            return;
        }

        removeTab(index);
        tab->deleteLater();

        if (!count()) {
            // TODO: add option to not close window when
            //       last tab is closed
            emit windowCloseRequested();
        }
    });
}

int TabWidget::addTab(Tab *tab, const QString &label)
{
    WebView *webView = tab->webView();

    connect(webView, &WebView::titleChanged, this, [this, tab] (const QString &title) {
        int index = indexOf(tab);
        setTabText(index, title);
        setTabToolTip(index, title);
    });

    connect(webView, &WebView::iconChanged, this, [this, tab] (const QIcon &icon) {
        int index = indexOf(tab);
        setTabIcon(index, icon);
    });

    connect(webView, &WebView::urlChanged, this, [this, tab] (const QUrl &url) {
        int index = indexOf(tab);
        if (index != currentIndex()) {
            return ;
        }
        emit urlChanged(url);
    });

    return QTabWidget::addTab(tab, label);
}

void TabWidget::back()
{
    Tab *tab = dynamic_cast<Tab *>(widget(currentIndex()));
    if (!tab) {
        return;
    }

    tab->webView()->back();
}

void TabWidget::forward()
{
    Tab *tab = dynamic_cast<Tab *>(widget(currentIndex()));
    if (!tab) {
        return;
    }

    tab->webView()->forward();
}
