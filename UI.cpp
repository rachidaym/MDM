#include "UI.h"

UI::UI() {
	open = true;
}

void UI::Show(std::vector<Object*>& objects, Object*& selected, Vector2 center) {
	for (int i = 0; i < windows.size();i++) {
		if (windows[i]->open) {
			windows[i]->Show(objects, selected, center, totalArea, windows);
		}
		else {
			windows.erase(windows.begin()+i);
		}
	}
	ImGui::Begin("MAIN", &open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove);
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(0.25*GetScreenWidth(), GetScreenHeight()));
	if (ImGui::Button("Create Object", ImVec2(0.95*ImGui::GetWindowWidth(), 0.1 * ImGui::GetWindowHeight()))) {
		windows.push_back(new ObjectCreator());
	}
	if (ImGui::Button("Calculate Moment of Inertia", ImVec2(0.95 * ImGui::GetWindowWidth(), 0.1 * ImGui::GetWindowHeight()))) {
		windows.push_back(new MomentCalculator());
	}
	
	ImGui::SeparatorText("Information");
	ImGui::Text((std::string("Total Area = ") + std::to_string(totalArea)).c_str());
	ImGui::Text((std::string("Xc = ") + std::to_string(center.x)).c_str());
	ImGui::Text((std::string("Yc = ") + std::to_string(center.y)).c_str());
	
	ImGui::SeparatorText("Object List");
	ImGui::BeginListBox("##hidelabel", ImVec2(0.95*ImGui::GetWindowWidth(), 140));
	totalArea = 0;
	for (auto o : objects) {
		totalArea += o->area;
		if (ImGui::Button(o->name.c_str(), ImVec2(0.94*ImGui::GetWindowWidth(), 20))) {
			if (selected == o) {
				selected = nullptr;
			}
			else {
				selected = o;
			}
		}
	}
	ImGui::EndListBox();
	if (selected) {
		if (ImGui::Button("Delete Selected Object", ImVec2(0.95*ImGui::GetWindowWidth(), 20))) {
			for (int i = 0;i < objects.size();i++) {
				if (objects[i] == selected) {
					objects.erase(objects.cbegin() + i);
					delete selected;
					selected = nullptr;
					ImGui::End();
					return;
				}
			}
		}
		ImGui::SeparatorText("Selected Object Properties");
		ImGui::Text((std::string("Name : ") + selected->name).c_str());
		if (ImGui::Button("Invert", ImVec2(0.5 * ImGui::GetWindowWidth(), 20))) {
			selected->mass = -selected->mass;
		}
		ImGui::InputFloat("Position X", &selected->pos.x, 1, 5, "%.3f");
		ImGui::InputFloat("Poisition Y", &selected->pos.y, 1, 5, "%.3f");
		switch (selected->type) {
		case RECT:
			ImGui::InputFloat("Width", &(static_cast<Rect*>(selected)->width), 0.1, 0.5, "%.3f");
			ImGui::InputFloat("Height", &(static_cast<Rect*>(selected)->height), 0.1, 0.5, "%.3f");
			if (static_cast<Rect*>(selected)->width < 0) {
				static_cast<Rect*>(selected)->width = 0;
			}
			if (static_cast<Rect*>(selected)->height < 0) {
				static_cast<Rect*>(selected)->height = 0;
			}
			break;
		case TRIANGLE:
			ImGui::InputFloat("Base", &(static_cast<Triangle*>(selected)->b1), 0.1, 0.5, "%.3f");
			ImGui::InputFloat("Offset", &(static_cast<Triangle*>(selected)->b2), 0.1, 0.5, "%.3f");
			ImGui::InputFloat("Height", &(static_cast<Triangle*>(selected)->height), 0.1, 0.5, "%.3f");
			if (static_cast<Triangle*>(selected)->b1 < 0) {
				static_cast<Triangle*>(selected)->b1 = 0;
			}
			if (static_cast<Triangle*>(selected)->height < 0) {
				static_cast<Triangle*>(selected)->height = 0;
			}
			break;
		case TRAPEZOID:
			ImGui::InputFloat("Bottom Base", &(static_cast<Trapezoid*>(selected)->bottomBase), 0.1, 0.5, "%.3f");
			ImGui::InputFloat("Top Base", &(static_cast<Trapezoid*>(selected)->topBase), 0.1, 0.5, "%.3f");
			ImGui::InputFloat("Height", &(static_cast<Trapezoid*>(selected)->h), 0.1, 0.5, "%.3f");
			if (static_cast<Trapezoid*>(selected)->topBase < 0) {
				static_cast<Trapezoid*>(selected)->topBase = 0;
			}
			if (static_cast<Trapezoid*>(selected)->bottomBase < 0) {
				static_cast<Trapezoid*>(selected)->bottomBase = 0;
			}
			if (static_cast<Trapezoid*>(selected)->h < 0) {
				static_cast<Trapezoid*>(selected)->h = 0;
			}
			if (static_cast<Trapezoid*>(selected)->topBase > static_cast<Trapezoid*>(selected)->bottomBase) {
				static_cast<Trapezoid*>(selected)->topBase = static_cast<Trapezoid*>(selected)->bottomBase;
			}
			break;
		case CIRCLE:
			ImGui::InputFloat("Inner Radius", &(static_cast<Circle*>(selected)->innerRadius), 0.1, 0.5, "%.3f");
			ImGui::InputFloat("Outer Radius", &(static_cast<Circle*>(selected)->outerRadius), 0.1, 0.5, "%.3f");
			ImGui::SliderFloat("Start Angle", &(static_cast<Circle*>(selected)->secondAngle), 0, 360, "%.3f");
			ImGui::SliderFloat("End Angle", &(static_cast<Circle*>(selected)->firstAngle), 0, 360, "%.3f");
			if (static_cast<Circle*>(selected)->innerRadius > static_cast<Circle*>(selected)->outerRadius) {
				static_cast<Circle*>(selected)->innerRadius = static_cast<Circle*>(selected)->outerRadius;
			}
			if (static_cast<Circle*>(selected)->outerRadius < 0) {
				static_cast<Circle*>(selected)->outerRadius = 0;
			}
			break;
		}
		ImGui::SeparatorText("Selected Object Information");
		ImGui::Text((std::string("Area = ") + std::to_string(selected->area)).c_str());
		ImGui::Text((std::string("Xc = ") + std::to_string(selected->center.x)).c_str());
		ImGui::Text((std::string("Yc = ") + std::to_string(selected->center.y)).c_str());
	}
	ImGui::End();
}


void ObjectCreator::Show(std::vector<Object*>& objects, Object*& selected, Vector2 center, float area, std::vector<Window*>& windows) {
	ImGui::Begin("Create Object", &open, ImGuiWindowFlags_NoMove);
	ImGui::SetWindowPos(ImVec2(GetScreenWidth() / 2.2, GetScreenHeight() / 3));
	ImGui::SetWindowSize(ImVec2(300, 120));
	const char* list[] = { "Rectangle", "Triangle", "Circle", "Trapezoid" };
	ImGui::Combo("Select Type", &selection, list, IM_ARRAYSIZE(list));
	ImGui::InputText("Object Name", name, 20);
	if (ImGui::Button("Create")) {
		switch (selection) {
		case 0:
			objects.push_back(new Rect(name,Vector2{ 0,0 }, 2, 3));
			break;
		case 1:
			objects.push_back(new Triangle(name,Vector2{ 0,0 }, 1, 2, 0));
			break;
		case 2:
			objects.push_back(new Circle(name,Vector2{ 0,0 }, 0, 1, 0, 360));
			break;
		case 3:
			objects.push_back(new Trapezoid(name,Vector2{ 0,0 }, 2, 3, 2));
			break;
		}
		selected = objects.back();
		open = false;
	}
	ImGui::End();
}

void MomentCalculator::Show(std::vector<Object*>& objects, Object*& selected, Vector2 center, float area, std::vector<Window*>& windows) {
	ImGui::Begin("Calculate Moment", &open);
	ImGui::SetWindowPos(ImVec2(GetScreenWidth() / 2.2, GetScreenHeight() / 3));
	ImGui::SetWindowSize(ImVec2(300, 80));
	
	ImGui::SliderAngle("Axis Angle", &axisAngle, -90, 90, "%.2f deg", ImGuiSliderFlags_AlwaysClamp);
	if (ImGui::Button("Calculate")) {
		Calculate(objects, center, area);
		windows.push_back(new ShowResults(*this));
		open = false;
	}
	ImGui::End();
}

void MomentCalculator::Calculate(std::vector<Object*>& objects, Vector2 center, float area) {
	Ix = 0;
	Iy = 0;
	Ixy = 0;
	Ixc = 0;
	Iyc = 0;
	Ixyc = 0;
	Ixa = 0;
	Iya = 0;
	Ixya = 0;
	Ip1 = 0;
	Ip2 = 0;
	principalAngle = 0;
	for (auto o : objects) {
		o->CalculateMOI();
		Ix += o->Ix + o->center.y * o->center.y * o->area;
		Iy += o->Iy + o->center.x * o->center.x * o->area;
		Ixy += o->Ixy + o->center.x * o->center.y * o->area;

		Ixc = Ix - center.y * center.y * area;
		Iyc = Iy - center.x * center.x * area;
		Ixyc = Ixy - center.x * center.y * area;

		float a = (Ixc + Iyc) / 2;
		float b = (Ixc - Iyc) / 2;
		Ixa = a + b * cos(2 * axisAngle) - Ixyc * sin(2 * axisAngle);
		Iya = a - b * cos(2 * axisAngle) - Ixyc * sin(2 * axisAngle);
		Ixya = b * sin(2 * axisAngle) + Ixyc * cos(2 * axisAngle);

		float c = sqrt(b*b + Ixyc*Ixyc);
		Ip1 = a + c;
		Ip2 = a - c;
		float d = -(2 * Ixyc) / (Ixc - Iyc);
		principalAngle = (atan(d)*180)/(2*PI);
	}
}

ShowResults::ShowResults(MomentCalculator m) {
	moment = m;
}

void ShowResults::Show(std::vector<Object*>& objects, Object*& selected, Vector2 center, float area, std::vector<Window*>& windows) {
	ImGui::Begin("Results", &open);
	//ImGui::SetWindowPos(ImVec2(GetScreenWidth() / 2.2, GetScreenHeight() / 3));
	ImGui::SetWindowSize(ImVec2(350, 350));

	ImGui::Text((std::string("Ix = ") + std::to_string(moment.Ix)).c_str());
	ImGui::Text((std::string("Iy = ") + std::to_string(moment.Iy)).c_str());
	ImGui::Text((std::string("Ixy = ") + std::to_string(moment.Ixy)).c_str());

	ImGui::SeparatorText("Central Axis");

	ImGui::Text((std::string("Ixc = ") + std::to_string(moment.Ixc)).c_str());
	ImGui::Text((std::string("Iyc = ") + std::to_string(moment.Iyc)).c_str());
	ImGui::Text((std::string("Ixyc = ") + std::to_string(moment.Ixyc)).c_str());

	if (moment.axisAngle != 0) {
		ImGui::SeparatorText((std::string("Central Axis with angle = ") + std::to_string(moment.axisAngle*180/PI)).c_str());
		ImGui::Text((std::string("Ixc = ") + std::to_string(moment.Ixa)).c_str());
		ImGui::Text((std::string("Iyc = ") + std::to_string(moment.Iya)).c_str());
		ImGui::Text((std::string("Ixyc = ") + std::to_string(moment.Ixya)).c_str());
	}

	ImGui::SeparatorText("Principal Axis");
	ImGui::Text((std::string("I1 = ") + std::to_string(moment.Ip1)).c_str());
	ImGui::Text((std::string("I2 = ") + std::to_string(moment.Ip2)).c_str());
	ImGui::Text((std::string("alpha = ") + std::to_string(moment.principalAngle)).c_str());

	ImGui::End();
}