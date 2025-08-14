// ==================== EngineGUI para ChrisEngine ====================
class EngineGUI {
    Theme m_currentTheme = Theme::Dark;
    bool m_paused = false;
    bool m_requestQuit = false;
    bool m_requestReset = false;
    float m_speedMultiplier = 1.0f;
    std::vector<TSharedPointer<A_Racer>> m_racers;

public:
    // Inicialización de ImGui-SFML
    void init(const TSharedPointer<Window>& window) {
        ImGui::SFML::Init(window->getInternal());
        setTheme(m_currentTheme);
    }

    // Actualización de UI por frame
    void update(const TSharedPointer<Window>& window, sf::Time deltaTime, float raceTimer) {
        ImGui::SFML::Update(window->getInternal(), deltaTime);

        renderMenuBar();
        renderControlPanel();

        // Stats de carrera
        ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration);
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Timer: %.2f s", raceTimer);
        ImGui::End();

        // Ventana de corredores/podio
        ImGui::Begin("Racers / Podio", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        auto sorted = m_racers; // copia local
        std::sort(sorted.begin(), sorted.end(),
            [](auto& a, auto& b) { return a->getProgress() > b->getProgress(); });

        int idx = 1;
        for (auto& r : sorted) {
            std::string label = std::to_string(idx) + ". " + r->getName() +
                " (P" + std::to_string(r->getPlace() ? r->getPlace() : idx) + ")";
            char buf[32];
            std::snprintf(buf, 32, "%.1f%%", r->getProgress() * 100.f);
            ImGui::Text("%s %s", label.c_str(), buf);

            if (ImGui::SmallButton(("Reset##" + std::to_string(idx)).c_str()))
                r->reset();
            idx++;
        }
