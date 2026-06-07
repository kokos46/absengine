#include <QActionGroup>
#include <QApplication>
#include <QBrush>
#include <QColor>
#include <QDockWidget>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <QWheelEvent>

#include <algorithm>
#include <cmath>
#include <functional>

namespace {

constexpr int ItemKindRole = 0;
constexpr int SurfaceKind = 1;
constexpr int TriggerKind = 2;
constexpr int PlayerKind = 3;

QString kindName(int kind)
{
    switch (kind) {
    case SurfaceKind:
        return "Platform";
    case TriggerKind:
        return "Trigger";
    case PlayerKind:
        return "Player";
    default:
        return "Object";
    }
}

QColor kindColor(int kind)
{
    switch (kind) {
    case SurfaceKind:
        return QColor(70, 76, 86);
    case TriggerKind:
        return QColor(230, 142, 40, 120);
    case PlayerKind:
        return QColor(54, 128, 218);
    default:
        return QColor(120, 120, 120);
    }
}

class LevelItem final : public QGraphicsRectItem {
public:
    LevelItem(int kind, const QRectF& rect)
        : QGraphicsRectItem(rect)
    {
        setData(ItemKindRole, kind);
        setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
        setBrush(QBrush(kindColor(kind)));
        setPen(QPen(kind == TriggerKind ? QColor(210, 100, 20) : QColor(35, 38, 44), 2.0));
        setToolTip(kindName(kind));
    }
};

class LevelScene final : public QGraphicsScene {
public:
    explicit LevelScene(QObject* parent = nullptr)
        : QGraphicsScene(parent)
    {
        setSceneRect(0, 0, 1600, 900);
    }

protected:
    void drawBackground(QPainter* painter, const QRectF& rect) override
    {
        QGraphicsScene::drawBackground(painter, rect);

        constexpr int grid = 32;
        QPen finePen(QColor(228, 231, 235));
        QPen axisPen(QColor(188, 194, 204));

        const int left = static_cast<int>(std::floor(rect.left() / grid)) * grid;
        const int top = static_cast<int>(std::floor(rect.top() / grid)) * grid;

        for (int x = left; x < rect.right(); x += grid) {
            painter->setPen(x == 0 ? axisPen : finePen);
            painter->drawLine(QLineF(x, rect.top(), x, rect.bottom()));
        }

        for (int y = top; y < rect.bottom(); y += grid) {
            painter->setPen(y == 0 ? axisPen : finePen);
            painter->drawLine(QLineF(rect.left(), y, rect.right(), y));
        }
    }
};

class LevelView final : public QGraphicsView {
public:
    using AddCallback = std::function<void(const QPointF&)>;

    explicit LevelView(QWidget* parent = nullptr)
        : QGraphicsView(parent)
    {
        setRenderHint(QPainter::Antialiasing);
        setDragMode(QGraphicsView::RubberBandDrag);
        setTransformationAnchor(AnchorUnderMouse);
        setBackgroundBrush(QColor(248, 249, 251));
    }

    void setAddCallback(AddCallback callback)
    {
        addCallback = std::move(callback);
    }

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        if (addCallback && event->button() == Qt::LeftButton) {
            addCallback(mapToScene(event->pos()));
            return;
        }

        QGraphicsView::mousePressEvent(event);
    }

    void wheelEvent(QWheelEvent* event) override
    {
        const double factor = event->angleDelta().y() > 0 ? 1.12 : 0.88;
        scale(factor, factor);
    }

private:
    AddCallback addCallback;
};

class MainWindow final : public QMainWindow {
public:
    MainWindow()
    {
        setWindowTitle("AbsEngine Level Editor");
        resize(1200, 760);

        scene = new LevelScene(this);
        view = new LevelView(this);
        view->setScene(scene);
        setCentralWidget(view);

        buildActions();
        buildPropertiesDock();

        connect(scene, &QGraphicsScene::selectionChanged, this, [this]() { syncPropertiesFromSelection(); });
        connect(scene, &QGraphicsScene::changed, this, [this]() { syncPropertiesFromSelection(); });
        setMode(SurfaceKind);
        newLevel();
    }

private:
    LevelScene* scene = nullptr;
    LevelView* view = nullptr;
    LevelItem* player = nullptr;
    QString currentPath;
    int currentMode = SurfaceKind;

    QSpinBox* xSpin = nullptr;
    QSpinBox* ySpin = nullptr;
    QSpinBox* wSpin = nullptr;
    QSpinBox* hSpin = nullptr;
    QLabel* selectedLabel = nullptr;

    void buildActions()
    {
        auto* fileMenu = menuBar()->addMenu("File");
        auto* toolBar = addToolBar("Tools");
        toolBar->setMovable(false);

        QAction* newAction = fileMenu->addAction("New");
        QAction* openAction = fileMenu->addAction("Open");
        QAction* saveAction = fileMenu->addAction("Save");
        QAction* saveAsAction = fileMenu->addAction("Save As");

        connect(newAction, &QAction::triggered, this, [this]() { newLevel(); });
        connect(openAction, &QAction::triggered, this, [this]() { openLevel(); });
        connect(saveAction, &QAction::triggered, this, [this]() { saveLevel(false); });
        connect(saveAsAction, &QAction::triggered, this, [this]() { saveLevel(true); });

        toolBar->addAction(newAction);
        toolBar->addAction(openAction);
        toolBar->addAction(saveAction);
        toolBar->addSeparator();

        auto* modeGroup = new QActionGroup(this);
        addModeAction(toolBar, modeGroup, "Platform", SurfaceKind, true);
        addModeAction(toolBar, modeGroup, "Trigger", TriggerKind, false);
        addModeAction(toolBar, modeGroup, "Player", PlayerKind, false);

        toolBar->addSeparator();
        QAction* selectAction = toolBar->addAction("Select");
        selectAction->setCheckable(true);
        modeGroup->addAction(selectAction);
        connect(selectAction, &QAction::triggered, this, [this]() {
            currentMode = 0;
            view->setAddCallback(nullptr);
            statusBar()->showMessage("Select and move objects", 2500);
        });

        QAction* deleteAction = toolBar->addAction("Delete");
        connect(deleteAction, &QAction::triggered, this, [this]() { deleteSelected(); });
    }

    void addModeAction(QToolBar* toolBar, QActionGroup* group, const QString& text, int kind, bool checked)
    {
        QAction* action = toolBar->addAction(text);
        action->setCheckable(true);
        action->setChecked(checked);
        group->addAction(action);
        connect(action, &QAction::triggered, this, [this, kind]() { setMode(kind); });
    }

    void buildPropertiesDock()
    {
        auto* dock = new QDockWidget("Properties", this);
        auto* panel = new QWidget(dock);
        auto* layout = new QVBoxLayout(panel);

        selectedLabel = new QLabel("Nothing selected");
        layout->addWidget(selectedLabel);

        xSpin = addSpinBox(layout, "X");
        ySpin = addSpinBox(layout, "Y");
        wSpin = addSpinBox(layout, "Width");
        hSpin = addSpinBox(layout, "Height");

        layout->addStretch();
        dock->setWidget(panel);
        addDockWidget(Qt::RightDockWidgetArea, dock);

        const auto update = [this]() { applyPropertiesToSelection(); };
        connect(xSpin, &QSpinBox::valueChanged, this, update);
        connect(ySpin, &QSpinBox::valueChanged, this, update);
        connect(wSpin, &QSpinBox::valueChanged, this, update);
        connect(hSpin, &QSpinBox::valueChanged, this, update);
    }

    QSpinBox* addSpinBox(QVBoxLayout* layout, const QString& label)
    {
        auto* row = new QWidget;
        auto* rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(0, 0, 0, 0);

        auto* caption = new QLabel(label);
        auto* spin = new QSpinBox;
        spin->setRange(-100000, 100000);
        spin->setSingleStep(8);

        rowLayout->addWidget(caption);
        rowLayout->addWidget(spin);
        layout->addWidget(row);

        return spin;
    }

    void setMode(int kind)
    {
        currentMode = kind;
        view->setAddCallback([this](const QPointF& pos) { addItemAt(pos, currentMode); });
        statusBar()->showMessage("Click the scene to add " + kindName(kind).toLower(), 2500);
    }

    void addItemAt(const QPointF& pos, int kind)
    {
        if (kind == PlayerKind) {
            if (!player) {
                player = createItem(PlayerKind, QRectF(0, 0, 40, 40));
            }
            player->setPos(snap(pos));
            scene->clearSelection();
            player->setSelected(true);
            return;
        }

        const QSizeF size = kind == TriggerKind ? QSizeF(48, 48) : QSizeF(128, 32);
        auto* item = createItem(kind, QRectF(0, 0, size.width(), size.height()));
        item->setPos(snap(pos));
        scene->clearSelection();
        item->setSelected(true);
    }

    LevelItem* createItem(int kind, const QRectF& rect)
    {
        auto* item = new LevelItem(kind, rect);
        scene->addItem(item);
        return item;
    }

    QPointF snap(const QPointF& pos) const
    {
        constexpr qreal grid = 8.0;
        return QPointF(std::round(pos.x() / grid) * grid, std::round(pos.y() / grid) * grid);
    }

    LevelItem* selectedLevelItem() const
    {
        const auto selected = scene->selectedItems();
        if (selected.size() != 1) {
            return nullptr;
        }

        return dynamic_cast<LevelItem*>(selected.first());
    }

    void syncPropertiesFromSelection()
    {
        LevelItem* item = selectedLevelItem();
        const bool enabled = item != nullptr;

        for (QSpinBox* spin : { xSpin, ySpin, wSpin, hSpin }) {
            spin->setEnabled(enabled);
        }

        if (!item) {
            selectedLabel->setText("Nothing selected");
            return;
        }

        const QRectF rect = item->sceneBoundingRect();
        selectedLabel->setText(kindName(item->data(ItemKindRole).toInt()));

        const QSignalBlocker bx(xSpin);
        const QSignalBlocker by(ySpin);
        const QSignalBlocker bw(wSpin);
        const QSignalBlocker bh(hSpin);

        xSpin->setValue(static_cast<int>(std::round(rect.x())));
        ySpin->setValue(static_cast<int>(std::round(rect.y())));
        wSpin->setValue(static_cast<int>(std::round(rect.width())));
        hSpin->setValue(static_cast<int>(std::round(rect.height())));
    }

    void applyPropertiesToSelection()
    {
        LevelItem* item = selectedLevelItem();
        if (!item) {
            return;
        }

        item->setRect(0, 0, std::max(1, wSpin->value()), std::max(1, hSpin->value()));
        item->setPos(xSpin->value(), ySpin->value());
    }

    void deleteSelected()
    {
        const auto selected = scene->selectedItems();
        for (QGraphicsItem* graphicsItem : selected) {
            if (graphicsItem == player) {
                player = nullptr;
            }
            delete graphicsItem;
        }
    }

    void newLevel()
    {
        scene->clear();
        player = createItem(PlayerKind, QRectF(0, 0, 40, 40));
        player->setPos(100, 300);
        createItem(SurfaceKind, QRectF(0, 0, 800, 50))->setPos(0, 400);
        currentPath.clear();
        syncPropertiesFromSelection();
        statusBar()->showMessage("New level", 2500);
    }

    void openLevel()
    {
        const QString path = QFileDialog::getOpenFileName(this, "Open level", QString(), "Level JSON (*.json)");
        if (path.isEmpty()) {
            return;
        }

        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Open level", "Cannot open file.");
            return;
        }

        const QJsonDocument document = QJsonDocument::fromJson(file.readAll());
        if (!document.isObject()) {
            QMessageBox::warning(this, "Open level", "Invalid level JSON.");
            return;
        }

        loadFromJson(document.object());
        currentPath = path;
        statusBar()->showMessage("Opened " + path, 2500);
    }

    void saveLevel(bool choosePath)
    {
        if (choosePath || currentPath.isEmpty()) {
            const QString path = QFileDialog::getSaveFileName(this, "Save level", currentPath.isEmpty() ? "level.json" : currentPath, "Level JSON (*.json)");
            if (path.isEmpty()) {
                return;
            }
            currentPath = path.endsWith(".json", Qt::CaseInsensitive) ? path : path + ".json";
        }

        QFile file(currentPath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QMessageBox::warning(this, "Save level", "Cannot write file.");
            return;
        }

        file.write(QJsonDocument(toJson()).toJson(QJsonDocument::Indented));
        statusBar()->showMessage("Saved " + currentPath, 2500);
    }

    QJsonObject rectToJson(const LevelItem* item) const
    {
        const QRectF rect = item->sceneBoundingRect();
        return {
            { "x", rect.x() },
            { "y", rect.y() },
            { "width", rect.width() },
            { "height", rect.height() },
        };
    }

    QJsonObject toJson() const
    {
        QJsonArray surfaces;
        QJsonArray triggers;

        for (QGraphicsItem* graphicsItem : scene->items()) {
            const auto* item = dynamic_cast<LevelItem*>(graphicsItem);
            if (!item || item == player) {
                continue;
            }

            const int kind = item->data(ItemKindRole).toInt();
            if (kind == SurfaceKind) {
                surfaces.append(rectToJson(item));
            } else if (kind == TriggerKind) {
                triggers.append(rectToJson(item));
            }
        }

        const QRectF playerRect = player ? player->sceneBoundingRect() : QRectF(100, 300, 40, 40);
        return {
            { "version", 1 },
            { "player", QJsonObject{
                { "x", playerRect.x() },
                { "y", playerRect.y() },
                { "width", playerRect.width() },
                { "height", playerRect.height() },
            } },
            { "surfaces", surfaces },
            { "triggers", triggers },
        };
    }

    void loadFromJson(const QJsonObject& root)
    {
        scene->clear();
        player = nullptr;

        const QJsonObject playerObject = root.value("player").toObject();
        player = createItem(PlayerKind, QRectF(0, 0, playerObject.value("width").toDouble(40), playerObject.value("height").toDouble(40)));
        player->setPos(playerObject.value("x").toDouble(100), playerObject.value("y").toDouble(300));

        loadItems(root.value("surfaces").toArray(), SurfaceKind);
        loadItems(root.value("triggers").toArray(), TriggerKind);
        syncPropertiesFromSelection();
    }

    void loadItems(const QJsonArray& array, int kind)
    {
        for (const QJsonValue& value : array) {
            const QJsonObject object = value.toObject();
            auto* item = createItem(kind, QRectF(0, 0, object.value("width").toDouble(64), object.value("height").toDouble(32)));
            item->setPos(object.value("x").toDouble(), object.value("y").toDouble());
        }
    }
};

} // namespace

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
