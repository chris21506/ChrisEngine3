// EngineGUIChrisEngine2.h/cpp
#include "EngineGUIChrisEngine2.h"
#include "Window.h"
#include "A_RacerChrisEngine2.h"
#include <imgui-SFML.h>
#include <algorithm>

// Inicializa ImGui-SFML y aplica el tema actual
void EngineGUIChrisEngine2::init(const EngineUtilities::TSharedPointer<Window>& window)
{
    ImGui::SFML::Init(window->getInternal());
    setTheme(m_currentTheme);
}

// Actualiza la UI cada frame
void EngineGUIChrisEngine2::update(const EngineUtilities::TSharedPointer<Window>& window,
    sf::Time deltaTime,
    float raceTimer)
{
    ImGui::SFML::Update(window->getInternal(), deltaTime);

    renderMenuBar();
    renderControlPanel();

    // Ventana de estadísticas
    ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Race Timer: %.2f s", raceTimer);
    ImGui::End();

    // Ventana de corredores/podio
    ImGui::Begin("Racers / Podium", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    auto sorted = m_racers;
    std::sort(sorted.begin(), sorted.end(),
        [](auto& a, auto& b) { return a->getProgress() > b->getProgress(); });

    int idx = 1;
    for (auto& r : sorted)
    {
        // Nombre + lugar en carrera
        std::string label = std::to_string(idx) + ". " + r->getName() +
            " (P" + std::to_string(r->getPlace() ? r->getPlace() : idx) + ")";
        char buf[32];
        std::snprintf(buf, 32, "%.1f%%", r->getProgress() * 100.f);
        ImGui::Text("%s %s", label.c_str(), buf);

        // Botón para reiniciar este corredor
        if (ImGui::SmallButton(("Reset##" + std::to_string(idx)).c_str()))
            r->reset();
        idx++;
    }

    ImGui::End();
}

// Renderiza ImGui
void EngineGUIChrisEngine2::render(const EngineUtilities::TSharedPointer<Window>& window)
{
    ImGui::SFML::Render(window->getInternal());
}

// Libera recursos de ImGui-SFML
void EngineGUIChrisEngine2::destroy()
{
    ImGui::SFML::Shutdown();
}

// Reenvía eventos SFML a ImGui
void EngineGUIChrisEngine2::processEvent(const EngineUtilities::TSharedPointer<Window>& window,
    const sf::Event& event)
{
    ImGui::SFML::ProcessEvent(window->getInternal(), event);
}

// Cambia el tema
void EngineGUIChrisEngine2::setTheme(Theme theme)
{
    m_currentTheme = theme;
    switch (theme)
    {
    case Theme::Grey: setupGreyGUIStyle(); break;
    case Theme::Dark: setupDarkGUIStyle(); break;
    case Theme::ChrisEngine2: setupChrisEngine2Style(); break;
    }
}

// Menú principal
void EngineGUIChrisEngine2::renderMenuBar()
{
    if (!ImGui::BeginMainMenuBar()) return;

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Exit")) m_requestQuit = true;
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Game"))
    {
        if (ImGui::MenuItem(m_paused ? "Resume" : "Pause")) m_paused = !m_paused;
        if (ImGui::MenuItem("Reset")) m_requestReset = true;
        ImGui::Separator();
        ImGui::Text("Speed:");
        ImGui::SliderFloat("##speed", &m_speedMultiplier, 0.1f, 3.0f, "%.2f");
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Theme"))
    {
        const char* names[] = { "Grey","Dark","ChrisEngine2" };
        for (int i = 0; i < 3; i++)
            if (ImGui::MenuItem(names[i], nullptr, m_currentTheme == Theme(i)))
                setTheme(Theme(i));
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

// Panel lateral de controles
void EngineGUIChrisEngine2::renderControlPanel()
{
    ImGui::Begin("Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    const char* names[] = { "Grey","Dark","ChrisEngine2" };
    int cur = int(m_currentTheme);
    if (ImGui::Combo("Theme", &cur, names, 3)) setTheme(Theme(cur));

    if (ImGui::Button(m_paused ? "Resume" : "Pause")) m_paused = !m_paused;
    ImGui::SameLine();
    if (ImGui::Button("Reset All")) m_requestReset = true;

    ImGui::SliderFloat("Speed Mul", &m_speedMultiplier, 0.1f, 3.f, "%.2f");

    if (ImGui::Button("Exit")) m_requestQuit = true;

    ImGui::End();
}

// Estilos
void EngineGUIChrisEngine2::setupGreyGUIStyle() { ImGui::StyleColorsClassic(); }
void EngineGUIChrisEngine2::setupDarkGUIStyle() { ImGui::StyleColorsDark(); }

// Estilo ChrisEngine-2 personalizado
void EngineGUIChrisEngine2::setupChrisEngine2Style()
{
    ImGui::StyleColorsDark();
    auto& s = ImGui::GetStyle();
    s.WindowRounding = 6.f; s.FrameRounding = 4.f;

    // Color acento dorado
    ImVec4 gold{ 0.92f,0.75f,0.20f,1.0f };
    s.Colors[ImGuiCol_ButtonHovered] = gold;
    s.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.65f, 0.18f, 1.0f);
    s.Colors[ImGuiCol_Border] = gold;

    // Aquí puedes ajustar más colores según identidad ChrisEngine-2
}
