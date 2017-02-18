while (true)
{
	//Všechna volání funkcí musí být mezi NewFrame a Render
	ImGui::NewFrame();

	//Umístí levý horní roh okna 100 pixelů od horního levého okraje obrazovky
	ImGui::SetNextWindowPos(ImVec2(100.0f, 100.0f);
	if (ImGui::Begin("Window"))
	{
		if (ImGui::Button("Button"))
			ImGui::Text("Foo");
		else
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Bar");
		ImGui::End();//Aby bylo vidět, kdo končí okno a může začít další
	}

	//ImGui si poznamenává všechny vykreslené objekty
	//a v této funkci je předá OpenGL pro vykreslení
	ImGui::Render();
}