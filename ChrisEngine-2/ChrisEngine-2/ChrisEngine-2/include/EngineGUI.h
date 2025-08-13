#pragma once

/**
 * @file ChrisEngine2GUI.h
 * @brief Declares the ChrisEngine2GUI class: ImGui-based HUD/menus for ChrisEngine-2.
 *
 * - Administra GUI, HUD, control de carrera y lista de corredores.
 * - Incluye personajes: princesa.png, sonic.png, virdo.png, wario.png.
 * - La pista se llama: pista de carreras.png.
 */

#include "Prerequisites.h"
#include <SFML/System.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>

class Window;
class ChrisEngine2; // Clase de los corredores adaptada

/**
 * @class ChrisEngine2GUI
 * @brief GUI de modo inmediato usando ImGui-SFML: menús, panel de control, estadísticas y lista de corredores.
 */
class ChrisEngine2GUI {
public:
    /**
     * @brief Temas visuales premade para la GUI.
     */
    enum class Theme { Grey = 0, Dark = 1, ChrisEngine2Theme = 2 };

    /**
     * @brief Constructor por defecto.
     */
    ChrisEngine2GUI() = default;

    /**
     * @brief Destructor por defecto.
     */
    ~ChrisEngine2GUI() = default;

    /**
     * @brief Inicializa ImGui-SFML con la ventana dada y establece tema por defecto.
     * @param window Ventana renderizable.
     */
    void init(const EngineUtilities::TSharedPointer<Window>& window);

    /**
     * @brief Actualización por frame y construcción de widgets.
     * @param window Ventana renderizable.
     * @param deltaTime Tiempo transcurrido desde el último frame.
     * @param raceTimer Tiempo acumulado de la carrera (HUD).
     */
    void update(const EngineUtilities::TSharedPointer<Window>& window,
        sf::Time deltaTime,
        float raceTimer);

    /**
     * @brief Renderiza los draw data de ImGui en la ventana.
     * @param window Ventana renderizable.
     */
    void render(const EngineUtilities::TSharedPointer<Window>& window);

    /**
     * @brief Cierra ImGui-SFML y libera recursos de la GUI.
     */
    void destroy();

    /**
     * @brief Reenvía eventos SFML a ImGui (teclado, mouse, etc.).
     */
    void processEvent(const EngineUtilities::TSharedPointer<Window>& window,
        const sf::Event& event);

    /**
     * @brief Devuelve true si el usuario solicitó salir (File → Exit / botón).
     */
    bool shouldQuit() const { return m_requestQuit; }

    /**
     * @brief Devuelve el estado actual de pausa.
     */
    bool isPaused() const { return m_paused; }

    /**
     * @brief Devuelve true una vez si se solicitó un reset completo de waypoints.
     */
    bool shouldResetWaypoints() {
        if (m_requestReset) { m_requestReset = false; return true; }
        return false;
    }

    /**
     * @brief Multiplicador de velocidad aplicado a la simulación (0.1x–3.0x).
     */
    float getSpeedMultiplier() const { return m_speedMultiplier; }

    /**
     * @brief Define la lista de corredores a mostrar/controlar en la GUI.
     * @param racers Vector de corredores compartidos.
     */
    void setRacers(const std::vector<EngineUtilities::TSharedPointer<ChrisEngine2>>& racers) {
        m_racers = racers;
    }

    /**
     * @brief Aplica un tema diferente a la GUI (colores, redondeo).
     */
    void setTheme(Theme theme);

private:
    /**
     * @brief Construye la barra de menú principal (File/Game/Theme).
     */
    void renderMenuBar();

    /**
     * @brief Construye el panel lateral de control (pausa, reset, velocidad, salir).
     */
    void renderControlPanel();

    /**
     * @brief Aplica estilo GUI light/grey.
     */
    void setupGreyGUIStyle();

    /**
     * @brief Aplica estilo GUI dark.
     */
    void setupDarkGUIStyle();

    /**
     * @brief Aplica estilo GUI custom ChrisEngine2Theme.
     */
    void setupChrisEngine2Style();

private:
    bool m_requestQuit = false;           ///< Se solicitó salir
    bool m_requestReset = false;          ///< Se solicitó reset (consumible)
    bool m_paused = false;                ///< Pausa activada
    float m_speedMultiplier = 1.f;       ///< Multiplicador de velocidad
    Theme m_currentTheme = Theme::ChrisEngine2Theme; ///< Tema actual
    std::vector<EngineUtilities::TSharedPointer<ChrisEngine2>> m_racers; ///< Corredores en GUI
};
