#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(1.5);

	this->number_of_agent = 100;
	this->len_of_tail = 10;

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 450, true, true, true);
	this->fbo.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update() {

	vector<string> characters = { "A", "B", "C", "D", "E", "F",
		"G", "H", "I", "J", "K", "L",
		"M", "N", "O", "P", "Q", "R",
		"S", "T", "U", "V", "W", "X",
		"Y", "Z" };

	this->fbo.begin();
	ofClear(0);
	ofSetColor(39);

	string draw_character = characters[(int)(ofGetFrameNum() * 0.01) % characters.size()];
	this->font.drawString(draw_character, ofGetWidth() * 0.5 - this->font.stringWidth(draw_character) * 0.5, ofGetHeight() * 0.5 + this->font.stringHeight(draw_character) * 0.5);

	this->fbo.end();
	this->fbo.readToPixels(this->pixels);

	while (this->logs.size() < this->number_of_agent) {

		int x = ofRandom(ofGetWidth());
		int y = ofRandom(ofGetHeight());
		if (this->pixels.getColor(x, y) != ofColor(0, 0)) {

			deque<glm::vec2> log;
			log.push_front(glm::vec2(x, y));
			this->logs.push_back(log);
			this->directions.push_back(glm::normalize(glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1))));
			ofColor color;
			color.setHsb(ofRandom(255), 255, 200);
			this->colors.push_back(color);
		}
	}

	for (int i = 0; i < this->number_of_agent; i++) {

		glm::vec2 future;
		if (this->pixels.getColor((int)this->logs[i].front().x, (int)this->logs[i].front().y) == ofColor(0, 0)) {
		
			future = this->logs[i].front();
		}
		else {

			future = this->logs[i].front() + this->directions[i] * 5;
			int x = future.x;
			int y = future.y;

			while (x < 0 || x > ofGetWidth() || y < 0 || y > ofGetHeight() || this->pixels.getColor(x, y) == ofColor(0, 0)) {

				this->directions[i] = glm::normalize(glm::vec2(ofRandom(-1, 1), ofRandom(-1, 1)));

				future = this->logs[i].front() + this->directions[i] * 10;
				x = future.x;
				y = future.y;
			}
		}

		this->logs[i].push_front(future);
		while (this->logs[i].size() > this->len_of_tail) {

			this->logs[i].pop_back();
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->fbo.draw(0, 0);

	ofFill();
	for (int i = 0; i < this->number_of_agent; i++) {

		for (int l = 0; l < this->logs[i].size() - 1; l++) {

			ofSetColor(this->colors[i], ofMap(l, 0, this->len_of_tail, 255, 64));
			ofDrawLine(this->logs[i][l], this->logs[i][l + 1]);
		}

		ofSetColor(this->colors[i]);
		ofDrawCircle(this->logs[i].front(), 3);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}