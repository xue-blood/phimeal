
void initShapes();
void clearShapes();

void drawArc(int _angle_start, int _angle_end);
#define drawCircle() drawArc(0,360)
void ArcDraw();

void drawPieArc(int _angle_start, int _angle_end);
#define drawPie() drawPieArc(0,360)
void PieDraw();

void QuadDraw();