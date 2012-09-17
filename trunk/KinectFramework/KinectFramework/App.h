#pragma once
class App
{
public:
	App(void);
	~App(void);
	void setup();
	void update();
	void draw();
	void exit();
	void setGraphGame(Grafo graphgame);
	void run();
	void addSubsystem();
};

