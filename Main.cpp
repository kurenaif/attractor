# include <Siv3D.hpp>

const Point size(1024, 1024);
const double r = 6.0;

const double rr = 6.0;

const double alpha = 1.5;
const double beta = 1.5;

const double dt = 1 / 60.0;

Float2 Point2Float2(Point pos){
	return Float2(((double)pos.x / size.x - 0.5) * r, ((double)pos.y / size.y - 0.5)*r);
}

Point Float22Point(Float2 pos){
	return Point(size.x*(pos.x / r + 0.5), size.y*(pos.y / r + 0.5));
}

Float2 F(Float2 p){
	return Float2(-p.y, p.x - alpha*p.y*(p.y*p.y-beta));
	//return Float2(-p.y, p.x + alpha*p.y);
}

class Ball{
private:
	Float2 m_pos;
public:
	Ball(Float2 pos) :m_pos(pos){}
	void Calc(){
		Float2 k1 = F(m_pos)*dt;
		Float2 k2 = F(m_pos + k1 / 2.0)*dt;
		Float2 k3 = F(m_pos + k2 / 2.0)*dt;
		Float2 k4 = F(m_pos + k3)*dt;
		m_pos = m_pos + F(k1 + 2.0*k2 + 2.0*k3 + k4) / 6.0;
	}
	Point GetPos(){ return Float22Point(m_pos); }
};

Float2 GetPoint(Float2 pos){
	Float2 k1 = F(pos)*dt;
	Float2 k2 = F(pos + k1 / 2.0)*dt;
	Float2 k3 = F(pos + k2 / 2.0)*dt;
	Float2 k4 = F(pos + k3)*dt;
	return F(k1 + 2.0*k2 + 2.0*k3 + k4) / 6.0;
}

void Main()
{
	const Font font(30);


	Window::Resize(size);

	Window::SetTitle(L"attractor");

	std::vector<Ball> ball;

	const double MAX = 100;
	//for (int i = 0; i < MAX; ++i){
	//	for (int j = 0; j < MAX; ++j){
	//		ball.push_back(Float2(-rr / 2.0 + rr * i/MAX, -rr / 2.0 + rr * j/MAX));
	//	}
	//}
	bool isStart = false;
	while (System::Update())
	{
		if (Input::MouseR.pressed){ isStart = true; }
		if (!isStart) continue;
		// 現在のカーソルの位置
		const Point pos = Mouse::Pos();

		// カーソルの位置に黄色の円を書く
		Circle(pos, 15).draw(Palette::Yellow);

		if (Input::MouseL.pressed){
			ball.push_back(Ball(Point2Float2(pos)));
		}

		for (Ball &b : ball){
			b.Calc();
			Circle(b.GetPos(), 15).draw(Palette::Yellow);
		}

		const int MAX_A = 40;
		for (int i = 0; i < MAX_A; ++i){
			for (int j = 0; j < MAX_A; ++j){
				Float2 f = Float2(-r / 2.0 + r * i / MAX_A, -r / 2.0 + r * j / MAX_A);
				Float2 t = GetPoint(f);
				Point to = Point(t.x * 400.0, t.y*400.0);
				Line(Float22Point(f), Float22Point(f)+to).drawArrow();
			}
		}

		font(Profiler::FPS(), L"fps").draw(Point(0, 900));

		// カーソルの位置を数字で表示
		font(Point2Float2(pos)).draw();
	}
}
