#include "../../supremacy.hpp"
#include "../../dependencies/imgui/settings.hpp"

namespace {
    using config_t = sdk::c_config_system;

    using bool_field_t = bool config_t::*;
    using int_field_t = int config_t::*;
    using map_field_t = std::unordered_map<int, bool> config_t::*;

    struct weapon_aim_members_t {
        map_field_t target_hitbox{};
        map_field_t multi_point{};
        bool_field_t static_scale{};
        int_field_t head_scale{};
        int_field_t body_scale{};
        bool_field_t automatic_fire{};
        bool_field_t silent_aim{};
        int_field_t minimum_hitchance{};
        int_field_t minimum_damage{};
        bool_field_t automatic_penetration{};
        int_field_t penetration_minimum_damage{};
        int_field_t override_minimum_damage{};
        bool_field_t automatic_scope{};
        int_field_t maximum_fov{};
    };

    struct weapon_other_members_t {
        bool_field_t remove_recoil{};
        int_field_t accuracy_boost{};
        bool_field_t quick_stop{};
        int_field_t quick_stop_type{};
        bool_field_t between_shots{};
        int_field_t lag_compensation{};
        map_field_t body_aim_conditions{};
        int_field_t max_misses_body_aim{};
        map_field_t safe_point_conditions{};
        int_field_t safe_point_type{};
        int_field_t max_misses_safe_point{};
        bool_field_t double_tap{};
        int_field_t max_shift_amount{};
    };

    constexpr std::size_t k_weapon_count = 10u;

    const std::vector<const char*> k_weapon_names = {
        "pistols",
        "heavy pistols",
        "sub-machine guns",
        "rifles",
        "auto snipers",
        "scout",
        "awp",
        "shotguns",
        "machine guns",
        "taser"
    };

    const std::vector<const char*> k_hitbox_items = {
        "head",
        "chest",
        "body",
        "arms",
        "legs",
        "feet"
    };

    const std::vector<const char*> k_quick_stop_types = {
        "full",
        "auto"
    };

    const std::vector<const char*> k_lag_compensation_items = {
        "off",
        "normal",
        "high"
    };

    const std::vector<const char*> k_body_aim_condition_items = {
        "prefer",
        "unsafe",
        "lethal",
        "lethal x2",
        "double shot",
        "overlapping desync",
        "max misses",
        "forward",
        "sideways"
    };

    const std::vector<const char*> k_safe_point_condition_items = {
        "prefer",
        "lethal",
        "lethal x2",
        "standing",
        "targeting limbs",
        "max misses",
        "forward",
        "sideways"
    };

    const weapon_aim_members_t k_weapon_aim_members[k_weapon_count] = {
        {
            &config_t::pistols_target_hitbox,
            &config_t::pistols_multi_point,
            &config_t::pistols_static_scale,
            &config_t::pistols_head_scale,
            &config_t::pistols_body_scale,
            &config_t::pistols_automatic_fire,
            &config_t::pistols_silent_aim,
            &config_t::pistols_minimum_hitchance,
            &config_t::pistols_minimum_damage,
            &config_t::pistols_automatic_penetration,
            &config_t::pistols_penetration_minimum_damage,
            &config_t::pistols_override_minimum_damage,
            nullptr,
            &config_t::pistols_maximum_fov
        },
        {
            &config_t::heavy_pistols_target_hitbox,
            &config_t::heavy_pistols_multi_point,
            &config_t::heavy_pistols_static_scale,
            &config_t::heavy_pistols_head_scale,
            &config_t::heavy_pistols_body_scale,
            &config_t::heavy_pistols_automatic_fire,
            &config_t::heavy_pistols_silent_aim,
            &config_t::heavy_pistols_minimum_hitchance,
            &config_t::heavy_pistols_minimum_damage,
            &config_t::heavy_pistols_automatic_penetration,
            &config_t::heavy_pistols_penetration_minimum_damage,
            &config_t::heavy_pistols_override_minimum_damage,
            nullptr,
            &config_t::heavy_pistols_maximum_fov
        },
        {
            &config_t::sub_machine_guns_target_hitbox,
            &config_t::sub_machine_guns_multi_point,
            &config_t::sub_machine_guns_static_scale,
            &config_t::sub_machine_guns_head_scale,
            &config_t::sub_machine_guns_body_scale,
            &config_t::sub_machine_guns_automatic_fire,
            &config_t::sub_machine_guns_silent_aim,
            &config_t::sub_machine_guns_minimum_hitchance,
            &config_t::sub_machine_guns_minimum_damage,
            &config_t::sub_machine_guns_automatic_penetration,
            &config_t::sub_machine_guns_penetration_minimum_damage,
            &config_t::sub_machine_guns_override_minimum_damage,
            nullptr,
            &config_t::sub_machine_guns_maximum_fov
        },
        {
            &config_t::rifles_target_hitbox,
            &config_t::rifles_multi_point,
            &config_t::rifles_static_scale,
            &config_t::rifles_head_scale,
            &config_t::rifles_body_scale,
            &config_t::rifles_automatic_fire,
            &config_t::rifles_silent_aim,
            &config_t::rifles_minimum_hitchance,
            &config_t::rifles_minimum_damage,
            &config_t::rifles_automatic_penetration,
            &config_t::rifles_penetration_minimum_damage,
            &config_t::rifles_override_minimum_damage,
            nullptr,
            &config_t::rifles_maximum_fov
        },
        {
            &config_t::auto_snipers_target_hitbox,
            &config_t::auto_snipers_multi_point,
            &config_t::auto_snipers_static_scale,
            &config_t::auto_snipers_head_scale,
            &config_t::auto_snipers_body_scale,
            &config_t::auto_snipers_automatic_fire,
            &config_t::auto_snipers_silent_aim,
            &config_t::auto_snipers_minimum_hitchance,
            &config_t::auto_snipers_minimum_damage,
            &config_t::auto_snipers_automatic_penetration,
            &config_t::auto_snipers_penetration_minimum_damage,
            &config_t::auto_snipers_override_minimum_damage,
            &config_t::auto_snipers_automatic_scope,
            &config_t::auto_snipers_maximum_fov
        },
        {
            &config_t::scout_target_hitbox,
            &config_t::scout_multi_point,
            &config_t::scout_static_scale,
            &config_t::scout_head_scale,
            &config_t::scout_body_scale,
            &config_t::scout_automatic_fire,
            &config_t::scout_silent_aim,
            &config_t::scout_minimum_hitchance,
            &config_t::scout_minimum_damage,
            &config_t::scout_automatic_penetration,
            &config_t::scout_penetration_minimum_damage,
            &config_t::scout_override_minimum_damage,
            &config_t::scout_automatic_scope,
            &config_t::scout_maximum_fov
        },
        {
            &config_t::awp_target_hitbox,
            &config_t::awp_multi_point,
            &config_t::awp_static_scale,
            &config_t::awp_head_scale,
            &config_t::awp_body_scale,
            &config_t::awp_automatic_fire,
            &config_t::awp_silent_aim,
            &config_t::awp_minimum_hitchance,
            &config_t::awp_minimum_damage,
            &config_t::awp_automatic_penetration,
            &config_t::awp_penetration_minimum_damage,
            &config_t::awp_override_minimum_damage,
            &config_t::awp_automatic_scope,
            &config_t::awp_maximum_fov
        },
        {
            &config_t::shotguns_target_hitbox,
            &config_t::shotguns_multi_point,
            &config_t::shotguns_static_scale,
            &config_t::shotguns_head_scale,
            &config_t::shotguns_body_scale,
            &config_t::shotguns_automatic_fire,
            &config_t::shotguns_silent_aim,
            &config_t::shotguns_minimum_hitchance,
            &config_t::shotguns_minimum_damage,
            &config_t::shotguns_automatic_penetration,
            &config_t::shotguns_penetration_minimum_damage,
            &config_t::shotguns_override_minimum_damage,
            nullptr,
            &config_t::shotguns_maximum_fov
        },
        {
            &config_t::machine_guns_target_hitbox,
            &config_t::machine_guns_multi_point,
            &config_t::machine_guns_static_scale,
            &config_t::machine_guns_head_scale,
            &config_t::machine_guns_body_scale,
            &config_t::machine_guns_automatic_fire,
            &config_t::machine_guns_silent_aim,
            &config_t::machine_guns_minimum_hitchance,
            &config_t::machine_guns_minimum_damage,
            &config_t::machine_guns_automatic_penetration,
            &config_t::machine_guns_penetration_minimum_damage,
            &config_t::machine_guns_override_minimum_damage,
            nullptr,
            &config_t::machine_guns_maximum_fov
        },
        {
            &config_t::taser_target_hitbox,
            &config_t::taser_multi_point,
            &config_t::taser_static_scale,
            &config_t::taser_head_scale,
            &config_t::taser_body_scale,
            &config_t::taser_automatic_fire,
            &config_t::taser_silent_aim,
            &config_t::taser_minimum_hitchance,
            &config_t::taser_minimum_damage,
            &config_t::taser_automatic_penetration,
            &config_t::taser_penetration_minimum_damage,
            &config_t::taser_override_minimum_damage,
            nullptr,
            &config_t::taser_maximum_fov
        }
    };

    const weapon_other_members_t k_weapon_other_members[k_weapon_count] = {
        {
            &config_t::pistols_remove_recoil,
            &config_t::pistols_accuracy_boost,
            &config_t::pistols_quick_stop,
            &config_t::pistols_type,
            &config_t::pistols_between_shots,
            &config_t::pistols_lag_compensation,
            &config_t::pistols_body_aim_conditions,
            &config_t::pistols_max_misses_body_aim,
            &config_t::pistols_safe_point_conditions,
            &config_t::pistols_safe_point_type,
            &config_t::pistols_max_misses_safe_point,
            &config_t::pistols_double_tap,
            &config_t::pistols_max_shift_amount
        },
        {
            &config_t::heavy_pistols_remove_recoil,
            &config_t::heavy_pistols_accuracy_boost,
            &config_t::heavy_pistols_quick_stop,
            &config_t::heavy_pistols_type,
            &config_t::heavy_pistols_between_shots,
            &config_t::heavy_pistols_lag_compensation,
            &config_t::heavy_pistols_body_aim_conditions,
            &config_t::heavy_pistols_max_misses_body_aim,
            &config_t::heavy_pistols_safe_point_conditions,
            &config_t::heavy_pistols_safe_point_type,
            &config_t::heavy_pistols_max_misses_safe_point,
            &config_t::heavy_pistols_double_tap,
            &config_t::heavy_pistols_max_shift_amount
        },
        {
            &config_t::sub_machine_guns_remove_recoil,
            &config_t::sub_machine_guns_accuracy_boost,
            &config_t::sub_machine_guns_quick_stop,
            &config_t::sub_machine_guns_type,
            &config_t::sub_machine_guns_between_shots,
            &config_t::sub_machine_guns_lag_compensation,
            &config_t::sub_machine_guns_body_aim_conditions,
            &config_t::sub_machine_guns_max_misses_body_aim,
            &config_t::sub_machine_guns_safe_point_conditions,
            &config_t::sub_machine_guns_safe_point_type,
            &config_t::sub_machine_guns_max_misses_safe_point,
            &config_t::sub_machine_guns_double_tap,
            &config_t::sub_machine_guns_max_shift_amount
        },
        {
            &config_t::rifles_remove_recoil,
            &config_t::rifles_accuracy_boost,
            &config_t::rifles_quick_stop,
            &config_t::rifles_type,
            &config_t::rifles_between_shots,
            &config_t::rifles_lag_compensation,
            &config_t::rifles_body_aim_conditions,
            &config_t::rifles_max_misses_body_aim,
            &config_t::rifles_safe_point_conditions,
            &config_t::rifles_safe_point_type,
            &config_t::rifles_max_misses_safe_point,
            &config_t::rifles_double_tap,
            &config_t::rifles_max_shift_amount
        },
        {
            &config_t::auto_snipers_remove_recoil,
            &config_t::auto_snipers_accuracy_boost,
            &config_t::auto_snipers_quick_stop,
            &config_t::auto_snipers_type,
            &config_t::auto_snipers_between_shots,
            &config_t::auto_snipers_lag_compensation,
            &config_t::auto_snipers_body_aim_conditions,
            &config_t::auto_snipers_max_misses_body_aim,
            &config_t::auto_snipers_safe_point_conditions,
            &config_t::auto_snipers_safe_point_type,
            &config_t::auto_snipers_max_misses_safe_point,
            &config_t::auto_snipers_double_tap,
            &config_t::auto_snipers_max_shift_amount
        },
        {
            &config_t::scout_remove_recoil,
            &config_t::scout_accuracy_boost,
            &config_t::scout_quick_stop,
            &config_t::scout_type,
            &config_t::scout_between_shots,
            &config_t::scout_lag_compensation,
            &config_t::scout_body_aim_conditions,
            &config_t::scout_max_misses_body_aim,
            &config_t::scout_safe_point_conditions,
            &config_t::scout_safe_point_type,
            &config_t::scout_max_misses_safe_point,
            &config_t::scout_double_tap,
            &config_t::scout_max_shift_amount
        },
        {
            &config_t::awp_remove_recoil,
            &config_t::awp_accuracy_boost,
            &config_t::awp_quick_stop,
            &config_t::awp_type,
            &config_t::awp_between_shots,
            &config_t::awp_lag_compensation,
            &config_t::awp_body_aim_conditions,
            &config_t::awp_max_misses_body_aim,
            &config_t::awp_safe_point_conditions,
            &config_t::awp_safe_point_type,
            &config_t::awp_max_misses_safe_point,
            &config_t::awp_double_tap,
            &config_t::awp_max_shift_amount
        },
        {
            &config_t::shotguns_remove_recoil,
            &config_t::shotguns_accuracy_boost,
            &config_t::shotguns_quick_stop,
            &config_t::shotguns_type,
            &config_t::shotguns_between_shots,
            &config_t::shotguns_lag_compensation,
            &config_t::shotguns_body_aim_conditions,
            &config_t::shotguns_max_misses_body_aim,
            &config_t::shotguns_safe_point_conditions,
            &config_t::shotguns_safe_point_type,
            &config_t::shotguns_max_misses_safe_point,
            &config_t::shotguns_double_tap,
            &config_t::shotguns_max_shift_amount
        },
        {
            &config_t::machine_guns_remove_recoil,
            &config_t::machine_guns_accuracy_boost,
            &config_t::machine_guns_quick_stop,
            &config_t::machine_guns_type,
            &config_t::machine_guns_between_shots,
            &config_t::machine_guns_lag_compensation,
            &config_t::machine_guns_body_aim_conditions,
            &config_t::machine_guns_max_misses_body_aim,
            &config_t::machine_guns_safe_point_conditions,
            &config_t::machine_guns_safe_point_type,
            &config_t::machine_guns_max_misses_safe_point,
            &config_t::machine_guns_double_tap,
            &config_t::machine_guns_max_shift_amount
        },
        {
            &config_t::taser_remove_recoil,
            &config_t::taser_accuracy_boost,
            &config_t::taser_quick_stop,
            &config_t::taser_type,
            &config_t::taser_between_shots,
            &config_t::taser_lag_compensation,
            &config_t::taser_body_aim_conditions,
            &config_t::taser_max_misses_body_aim,
            &config_t::taser_safe_point_conditions,
            &config_t::taser_safe_point_type,
            &config_t::taser_max_misses_safe_point,
            &config_t::taser_double_tap,
            &config_t::taser_max_shift_amount
        }
    };

    static_assert(std::size(k_weapon_aim_members) == std::size(k_weapon_other_members));
    static_assert(std::size(k_weapon_aim_members) == k_weapon_count);
}

namespace supremacy {

namespace {
    int clamp_weapon_index(const int index) {
        return std::clamp(index, 0, static_cast<int>(k_weapon_names.size()) - 1);
    }

    void ensure_indices(std::unordered_map<int, bool>& data, const std::vector<const char*>& items) {
        for (int i = 0; i < static_cast<int>(items.size()); ++i)
            data.try_emplace(i, false);
    }

    bool any_enabled(std::unordered_map<int, bool>& data, std::initializer_list<int> indices) {
        for (const auto idx : indices)
            if (data[idx])
                return true;

        return false;
    }

    void draw_weapon_aim_section(const int weapon_index) {
        auto& cfg = *sdk::g_config_system;
        const auto& members = k_weapon_aim_members[weapon_index];

        auto& target_hitbox = cfg.*members.target_hitbox;
        ensure_indices(target_hitbox, k_hitbox_items);
        ui::MultiSelect("target hitbox", &target_hitbox, k_hitbox_items);

        if (members.multi_point) {
            auto& multi_point = cfg.*members.multi_point;
            ensure_indices(multi_point, k_hitbox_items);
            ui::MultiSelect("multi-point", &multi_point, k_hitbox_items);

            if (members.static_scale && any_enabled(multi_point, { 0, 1, 2, 3, 4, 5 })) {
                auto& static_scale = cfg.*members.static_scale;
                ui::Checkbox("static scale", &static_scale);

                if (static_scale) {
                    if (members.head_scale && multi_point[0])
                        ui::SliderInt("head scale", &(cfg.*members.head_scale), 0, 100, "%d%%");

                    if (members.body_scale)
                        ui::SliderInt("body scale", &(cfg.*members.body_scale), 0, 100, "%d%%");
                }
            }
        }

        if (members.automatic_fire)
            ui::Checkbox("automatic fire", &(cfg.*members.automatic_fire));

        if (members.silent_aim)
            ui::Checkbox("silent aim", &(cfg.*members.silent_aim));

        if (members.minimum_hitchance)
            ui::SliderInt("minimum hitchance", &(cfg.*members.minimum_hitchance), 1, 99, "%d%%");

        if (members.minimum_damage)
            ui::SliderInt("minimum damage", &(cfg.*members.minimum_damage), 1, 100, "%dhp");

        if (members.automatic_penetration) {
            auto& automatic_penetration = cfg.*members.automatic_penetration;
            ui::Checkbox("automatic penetration", &automatic_penetration);

            if (automatic_penetration && members.penetration_minimum_damage)
                ui::SliderInt("penetration minimum damage", &(cfg.*members.penetration_minimum_damage), 1, 100, "%dhp");
        }

        ui::Text("override minimum damage");
        ui::Keybind("##override_minimum_damage_key", &cfg.override_minimum_damage_key, &cfg.override_minimum_damage_key_style);

        if (members.override_minimum_damage)
            ui::SliderInt("##override_minimum_damage", &(cfg.*members.override_minimum_damage), 1, 100, "%dhp");

        if (members.automatic_scope)
            ui::Checkbox("automatic scope", &(cfg.*members.automatic_scope));

        if (members.maximum_fov)
            ui::SliderInt("maximum fov", &(cfg.*members.maximum_fov), 1, 180, "%d�%");
    }

    bool should_show_safe_point_type(const std::unordered_map<int, bool>& data) {
        for (int i = 0; i <= 7; ++i)
            if (data.find(i) != data.end() && data.at(i))
                return true;

        return false;
    }

    void draw_weapon_other_section(const int weapon_index) {
        auto& cfg = *sdk::g_config_system;
        const auto& members = k_weapon_other_members[weapon_index];

        if (members.remove_recoil)
            ui::Checkbox("remove recoil", &(cfg.*members.remove_recoil));

        if (members.accuracy_boost)
            ui::SliderInt("accuracy boost", &(cfg.*members.accuracy_boost), 0, 100, "%d%%");

        if (members.quick_stop) {
            auto& quick_stop = cfg.*members.quick_stop;
            ui::Checkbox("quick stop", &quick_stop);

            if (quick_stop) {
                if (members.quick_stop_type)
                    ui::SingleSelect("##type", &(cfg.*members.quick_stop_type), k_quick_stop_types);

                if (members.between_shots)
                    ui::Checkbox("between shots", &(cfg.*members.between_shots));
            }
        }

        if (ui::Button(g_context->cvars().m_cl_lagcompensation->get_bool() ? "disable client lag compensation" : "enable client lag compensation")) {
            if (valve::g_engine->in_game() && valve::g_local_player)
                g_context->switch_lagcompensation_state() = true;
            else
                g_context->cvars().m_cl_lagcompensation->set_int(!g_context->cvars().m_cl_lagcompensation->get_bool());
        }

        if (members.lag_compensation && g_context->cvars().m_cl_lagcompensation->get_bool())
            ui::SingleSelect("lag compensation", &(cfg.*members.lag_compensation), k_lag_compensation_items);

        ui::Checkbox("anti-aim correction", &cfg.anti_aim_correction);

        if (members.body_aim_conditions) {
            auto& body_aim_conditions = cfg.*members.body_aim_conditions;
            ensure_indices(body_aim_conditions, k_body_aim_condition_items);
            ui::MultiSelect("body aim conditions", &body_aim_conditions, k_body_aim_condition_items);

            ui::Keybind("##body_aim_key", &cfg.body_aim_key, &cfg.body_aim_key_style);

            if (members.max_misses_body_aim && body_aim_conditions[6])
                ui::SliderInt("##max_misses_body_aim", &(cfg.*members.max_misses_body_aim), 1, 5, "%d miss");
        }

        if (members.safe_point_conditions) {
            auto& safe_point_conditions = cfg.*members.safe_point_conditions;
            ensure_indices(safe_point_conditions, k_safe_point_condition_items);
            ui::MultiSelect("safe point conditions", &safe_point_conditions, k_safe_point_condition_items);

            ui::Keybind("##safe_point_key", &cfg.safe_point_key, &cfg.safe_point_key_style);

            if (cfg.safe_point_key || should_show_safe_point_type(safe_point_conditions)) {
                if (members.safe_point_type)
                    ui::SingleSelect("type", &(cfg.*members.safe_point_type), { "normal", "strict" });

                if (members.max_misses_safe_point && safe_point_conditions[5])
                    ui::SliderInt("##max_misses_safe_point", &(cfg.*members.max_misses_safe_point), 1, 5, "%d miss");
            }
        }

        if (members.double_tap) {
            ui::Checkbox("double tap", &(cfg.*members.double_tap));
            int* mode = nullptr;
            if (members.double_tap == &config_t::pistols_double_tap) mode = &cfg.pistols_double_tap_mode;
            else if (members.double_tap == &config_t::heavy_pistols_double_tap) mode = &cfg.heavy_pistols_double_tap_mode;
            else if (members.double_tap == &config_t::sub_machine_guns_double_tap) mode = &cfg.sub_machine_guns_double_tap_mode;
            else if (members.double_tap == &config_t::rifles_double_tap) mode = &cfg.rifles_double_tap_mode;
            else if (members.double_tap == &config_t::auto_snipers_double_tap) mode = &cfg.auto_snipers_double_tap_mode;
            else if (members.double_tap == &config_t::scout_double_tap) mode = &cfg.scout_double_tap_mode;
            else if (members.double_tap == &config_t::awp_double_tap) mode = &cfg.awp_double_tap_mode;
            else if (members.double_tap == &config_t::shotguns_double_tap) mode = &cfg.shotguns_double_tap_mode;
            else if (members.double_tap == &config_t::machine_guns_double_tap) mode = &cfg.machine_guns_double_tap_mode;
            else if (members.double_tap == &config_t::taser_double_tap) mode = &cfg.taser_double_tap_mode;
            if (mode)
                ui::SingleSelect("dt mode", mode, {"normal", "fast", "very fast"});
        }

        ui::Keybind("##double_tap_key", &cfg.double_tap_key, &cfg.double_tap_key_style);

        if (members.max_shift_amount)
            ui::SliderInt("max shift amount", &(cfg.*members.max_shift_amount), 6, 15, "%d ticks");
    }
}
	void c_menu::init(IDirect3DDevice9* const pDevice) {
		if (this->m_initialized)
			return;

		ui::CreateContext();
		auto io = ui::GetIO();
		auto style = &ui::GetStyle();

		style->WindowRounding = 0.f;
		style->AntiAliasedLines = true;
		style->AntiAliasedFill = true;
		style->ScrollbarRounding = 2.f;
		style->ScrollbarSize = 8.f;
		style->WindowPadding = ImVec2(0, 0);
		style->FramePadding = ImVec2(8, 4);
		style->ItemSpacing = ImVec2(4, 3);
		style->ItemInnerSpacing = ImVec2(4, 2);
		style->Colors[ImGuiCol_ScrollbarBg] = ImColor(25, 25, 25, 255);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImColor(55, 55, 55, 255);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(70, 70, 70, 255);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(85, 85, 85, 255);
		style->Colors[ImGuiCol_WindowBg] = ImColor(18, 18, 18, 255);
		style->Colors[ImGuiCol_ChildBg] = ImColor(22, 22, 22, 255);
		style->Colors[ImGuiCol_Text] = ImColor(220, 220, 220, 255);
		style->Colors[ImGuiCol_Border] = ImColor(35, 35, 35, 255);
		style->Colors[ImGuiCol_FrameBg] = ImColor(28, 28, 28, 255);
		style->Colors[ImGuiCol_FrameBgHovered] = ImColor(35, 35, 35, 255);
		style->Colors[ImGuiCol_FrameBgActive] = ImColor(40, 40, 40, 255);

		settings::m_menu_color[0] = sdk::g_config_system->menu_color[0];
		settings::m_menu_color[1] = sdk::g_config_system->menu_color[1];
		settings::m_menu_color[2] = sdk::g_config_system->menu_color[2];
		settings::m_menu_color[3] = sdk::g_config_system->menu_color[3];
		this->m_is_opened = settings::m_is_opened = false;		

		IDirect3DSwapChain9* pChain = nullptr;
		D3DPRESENT_PARAMETERS pp = {};
		D3DDEVICE_CREATION_PARAMETERS param = {};
		pDevice->GetCreationParameters(&param);
		pDevice->GetSwapChain(0, &pChain);

		if (pChain)
			pChain->GetPresentParameters(&pp);

		ImGui_ImplWin32_Init(param.hFocusWindow);
		ImGui_ImplDX9_Init(pDevice);

		static const ImWchar ranges[] =	{
			0x0020u, 0x00ffu,
			0x0400u, 0x052fu,
			0x2de0u, 0x2dffu,
			0xa640u, 0xa69fu,
			0xe000u, 0xe226u,
			0u,
		};

		ImFontConfig cfg;
		cfg.RasterizerFlags = 1 << 7 | 1 << 4;
		cfg.GlyphRanges = ranges;

		io.Fonts->AddFontFromMemoryTTF(main_font, 291576, 12.f, &cfg, get_font_glyph_ranges(io.Fonts));
		io.Fonts->AddFontFromMemoryTTF(main_font, 291576, 12.f, &cfg, get_font_glyph_ranges(io.Fonts));
		io.Fonts->AddFontFromMemoryTTF(keybinds_font, 25600, 10.f, &cfg, get_font_glyph_ranges(io.Fonts));
		io.Fonts->AddFontDefault();

		ImGuiFreeType::BuildFontAtlas(io.Fonts, 0x00);

		this->m_initialized = true;
	}

	void c_menu::draw_begin(IDirect3DDevice9* const device)	{
		static std::once_flag fl;
		std::call_once(fl, [device]() {
			g_menu->init(device);
		});

		if (!this->m_initialized)
			return;

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ui::NewFrame();
	}

	void c_menu::draw()	{
		this->handle();

		if (!m_is_opened || ui::GetStyle().Alpha == 0.f)
			return;

		if (m_aimware_style) {
			draw_aimware_v5();
			return;
		}

		const auto& screen_size = ui::GetIO().DisplaySize;

		ui::SetNextWindowSizeConstraints(ImVec2(520, 580), ImVec2(screen_size.x - 2, screen_size.y - 2));
		
		const auto& menu_color = settings::m_menu_color;
		ui::PushStyleColor(ImGuiCol_Border, ImVec4(menu_color[0] * 0.35f, menu_color[1] * 0.35f, menu_color[2] * 0.35f, menu_color[3]));
		ui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.5f);
		ui::PushStyleVar(ImGuiStyleVar_WindowRounding, 4.f);
		
		ui::Begin("supremacy", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MainWindow);
		
		auto* window = ui::GetCurrentWindow();
		if (window && window->DrawList) {
			const auto& pos = window->Pos;
			const auto& size = window->Size;
			
			window->DrawList->AddRectFilledMultiColor(
				ImVec2(pos.x - 1, pos.y - 1), ImVec2(pos.x + size.x + 1, pos.y + size.y + 1),
				ImColor(15, 15, 15, 140), ImColor(18, 18, 18, 140),
				ImColor(18, 18, 18, 140), ImColor(15, 15, 15, 140)
			);
			
			window->DrawList->AddRect(
				ImVec2(pos.x - 1, pos.y - 1), ImVec2(pos.x + size.x + 1, pos.y + size.y + 1),
				ImColor(menu_color[0] * 0.25f, menu_color[1] * 0.25f, menu_color[2] * 0.25f, menu_color[3]), 
				4.f, 0, 2.f
			);
		}
		
		ui::TabButton("aimbot", &this->m_current_tab, 0, 6);
		ui::TabButton("anti-aim", &this->m_current_tab, 1, 6);
		ui::TabButton("visuals", &this->m_current_tab, 2, 6);
		ui::TabButton("misc", &this->m_current_tab, 3, 6);
		ui::TabButton("config", &this->m_current_tab, 4, 6);
		ui::TabButton("skins", &this->m_current_tab, 5, 6);
		
		ui::SameLine(ui::GetWindowWidth() - 130);
		if (ui::Button("Aimware Style", ImVec2(120, 25))) {
			m_aimware_style = true;
		}

		const auto child_size = ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y - 80 - 27);
		static auto calc_pos = [](int num) -> ImVec2 {
			if (num == 1)
				return ImVec2(ui::GetWindowPos().x - 4 + 26 + (ui::GetWindowSize().x / 2 - 36) * num + 20 * num, ui::GetWindowPos().y + 52 + 27);
			else
				return ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 2 - 36) * num + 20 * num, ui::GetWindowPos().y + 52 + 27);
		};

		if (this->m_current_tab == 0) {
			auto weapon_index = clamp_weapon_index(sdk::g_config_system->weapon_selection);

			ui::SetNextWindowPos(calc_pos(0));
			ui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.f);
			ui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.f);
			ui::PushStyleColor(ImGuiCol_ChildBg, ImColor(20, 20, 20, 255));
			ui::BeginChild("aimbot", child_size, true);
			{
				auto* child_window = ui::GetCurrentWindow();
				if (child_window && child_window->DrawList) {
					const auto& pos = child_window->Pos;
					const auto& size = child_window->Size;
					child_window->DrawList->AddRect(
						pos, ImVec2(pos.x + size.x, pos.y + size.y),
						ImColor(menu_color[0] * 0.25f, menu_color[1] * 0.25f, menu_color[2] * 0.25f, menu_color[3]), 
						6.f, 0, 1.5f
					);
					child_window->DrawList->AddRectFilledMultiColor(
						ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + 2),
						ImColor(menu_color[0], menu_color[1], menu_color[2], menu_color[3]),
						ImColor(menu_color[0] * 0.5f, menu_color[1] * 0.5f, menu_color[2] * 0.5f, menu_color[3]),
						ImColor(menu_color[0] * 0.5f, menu_color[1] * 0.5f, menu_color[2] * 0.5f, menu_color[3]),
						ImColor(menu_color[0], menu_color[1], menu_color[2], menu_color[3])
					);
				}
				ui::Spacing();
				ui::Checkbox("enabled", &sdk::g_config_system->enabled);
				ui::Keybind("##enabled_key", &sdk::g_config_system->enabled_key, &sdk::g_config_system->enabled_key_style);
				ui::Spacing();

				sdk::g_config_system->weapon_selection = weapon_index;
				ui::SingleSelect("weapon selection", &sdk::g_config_system->weapon_selection, k_weapon_names);
				weapon_index = clamp_weapon_index(sdk::g_config_system->weapon_selection);
				ui::Spacing();

				draw_weapon_aim_section(weapon_index);
				ui::Spacing();
				ui::Checkbox("log misses due to spread", &sdk::g_config_system->log_misses_due_to_spread);
			}
			ui::EndChild();
			ui::PopStyleColor();
			ui::PopStyleVar(2);

			ui::SetNextWindowPos(calc_pos(1));
			ui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.f);
			ui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.f);
			ui::PushStyleColor(ImGuiCol_ChildBg, ImColor(20, 20, 20, 255));
			ui::BeginChild("other", child_size, true);
			{
				auto* child_window = ui::GetCurrentWindow();
				if (child_window && child_window->DrawList) {
					const auto& pos = child_window->Pos;
					const auto& size = child_window->Size;
					child_window->DrawList->AddRect(
						pos, ImVec2(pos.x + size.x, pos.y + size.y),
						ImColor(menu_color[0] * 0.2f, menu_color[1] * 0.2f, menu_color[2] * 0.2f, menu_color[3]), 
						6.f, 0, 1.5f
					);
					child_window->DrawList->AddRectFilledMultiColor(
						ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + 2),
						ImColor(menu_color[0] * 0.8f, menu_color[1] * 0.8f, menu_color[2] * 0.8f, menu_color[3]),
						ImColor(menu_color[0] * 0.4f, menu_color[1] * 0.4f, menu_color[2] * 0.4f, menu_color[3]),
						ImColor(menu_color[0] * 0.4f, menu_color[1] * 0.4f, menu_color[2] * 0.4f, menu_color[3]),
						ImColor(menu_color[0] * 0.8f, menu_color[1] * 0.8f, menu_color[2] * 0.8f, menu_color[3])
					);
				}
				ui::Spacing();
				draw_weapon_other_section(weapon_index);
			}
			ui::EndChild();
			ui::PopStyleColor();
			ui::PopStyleVar(2);
		}

		if (this->m_current_tab == 1) {
			ui::SetNextWindowPos(calc_pos(0));
			ui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
			ui::PushStyleColor(ImGuiCol_ChildBg, ImColor(22, 22, 22, 255));
			ui::PushStyleColor(ImGuiCol_Border, ImColor(35, 35, 35, 255));
			ui::BeginChild("anti-aimbot angles", child_size, true);
			{
				auto* child_window = ui::GetCurrentWindow();
				if (child_window && child_window->DrawList) {
					const auto& pos = child_window->Pos;
					const auto& size = child_window->Size;
					child_window->DrawList->AddRect(
						pos, ImVec2(pos.x + size.x, pos.y + size.y),
						ImColor(menu_color[0] * 0.15f, menu_color[1] * 0.15f, menu_color[2] * 0.15f, menu_color[3]), 0.f, 0, 1.f
					);
				}
				ui::Checkbox("enabled", &sdk::g_config_system->enabled2);
				ui::SliderInt("pitch", &sdk::g_config_system->pitch, -89, 89, "%d�%");
				ui::SingleSelect("yaw base", &sdk::g_config_system->yaw_base, { "local view", "closest", "average", "fov" });
				ui::SliderInt("yaw", &sdk::g_config_system->yaw, -180, 180, "%d�%");
				ui::Checkbox("freestanding", &sdk::g_config_system->freestanding);
				ui::Keybind("##freestanding_key", &sdk::g_config_system->freestanding_key, &sdk::g_config_system->freestanding_key_style);
				ui::SliderInt("jitter range", &sdk::g_config_system->jitter_range, 0, 180, "%d�%");
				ui::SingleSelect("jitter switch", &sdk::g_config_system->jitter_switch, { "sending packet", "random" });
				ui::SliderInt("rotate range", &sdk::g_config_system->rotate_range, 0, 360, "%d�%");
				ui::SliderInt("rotate speed", &sdk::g_config_system->rotate_speed, 0, 100, "%d%%");
				ui::SingleSelect("body yaw", &sdk::g_config_system->body_yaw, { "off", "static", "jitter" });
				ui::Keybind("##body_yaw_key", &sdk::g_config_system->body_yaw_key, &sdk::g_config_system->body_yaw_key_style);
				if (sdk::g_config_system->body_yaw) {
					ui::SliderInt("yaw limit", &sdk::g_config_system->yaw_limit, 0, 58, "%d�%");
					ui::SliderInt("body lean", &sdk::g_config_system->body_lean, -180, 180, "%d�%");
					ui::SliderInt("inverted body lean", &sdk::g_config_system->inverted_body_lean, -180, 180, "%d�%");
					ui::SingleSelect("on shot side", &sdk::g_config_system->on_shot_side, { "off", "left", "right", "opposite", "switch" });
					if (sdk::g_config_system->body_yaw == 1)
						ui::SingleSelect("automatic side selection", &sdk::g_config_system->automatic_side_selection, { "off", "peek fake", "peek real" });
				}
				ui::Spacing();
				ui::Checkbox("custom desync", &sdk::g_config_system->custom_desync);
				if (sdk::g_config_system->custom_desync) {
					ui::SingleSelect("desync mode", &sdk::g_config_system->desync_mode, { "default", "jitter", "static", "random" });
					ui::SliderFloat("desync amount", &sdk::g_config_system->desync_amount, 0.f, 120.f, "%.1f°");
					if (sdk::g_config_system->desync_mode == 3)
						ui::SliderFloat("desync inverter", &sdk::g_config_system->desync_inverter, -1.f, 1.f, "%.2f");
				}
				ui::Spacing();
				const char* fake_peek_label = sdk::g_config_system->fake_peek_mode == 0 ? "fake peek" : "fake head";
				ui::Checkbox(fake_peek_label, &sdk::g_config_system->fake_peek);
				if (sdk::g_config_system->fake_peek) {
					ui::SingleSelect("name", &sdk::g_config_system->fake_peek_mode, { "fake peek", "fake head" });
					ui::SingleSelect("mode", &sdk::g_config_system->fake_peek_type, { "normal", "break" });
					ui::Keybind("##fake_peek_key", &sdk::g_config_system->fake_peek_key, &sdk::g_config_system->fake_peek_key_style);
				}
				ui::Spacing();
				ui::Text("manual left");
				ui::Keybind("##manual_left_key", &sdk::g_config_system->manual_left_key);
				ui::Text("manual right");
				ui::Keybind("##manual_right_key", &sdk::g_config_system->manual_right_key);
				ui::Text("manual back");
				ui::Keybind("##manual_back_key", &sdk::g_config_system->manual_back_key);
			}
			ui::EndChild();
			ui::PopStyleColor(2);
			ui::PopStyleVar();

			ui::SetNextWindowPos(calc_pos(1));
			ui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
			ui::PushStyleColor(ImGuiCol_ChildBg, ImColor(22, 22, 22, 255));
			ui::PushStyleColor(ImGuiCol_Border, ImColor(35, 35, 35, 255));
			ui::BeginChild("other", child_size, true);
			{
				auto* child_window = ui::GetCurrentWindow();
				if (child_window && child_window->DrawList) {
					const auto& pos = child_window->Pos;
					const auto& size = child_window->Size;
					child_window->DrawList->AddRect(
						pos, ImVec2(pos.x + size.x, pos.y + size.y),
						ImColor(menu_color[0] * 0.15f, menu_color[1] * 0.15f, menu_color[2] * 0.15f, menu_color[3]), 0.f, 0, 1.f
					);
				}
				ui::Checkbox("fake lag", &sdk::g_config_system->fake_lag);
				ui::SingleSelect("amount", &sdk::g_config_system->amount, { "dynamic", "maximal", "switch", "break lc" });
				ui::MultiSelect("conditions", &sdk::g_config_system->conditions, { "always", "on move", "in air" });
				ui::SliderInt("factor limit", &sdk::g_config_system->factor_limit, 0, 14, "%dticks");
				ui::Checkbox("force fake lag when peeking", &sdk::g_config_system->force_when_peeking);
				ui::Checkbox("hide shots", &sdk::g_config_system->hide_shots);
				ui::Keybind("##hide_shots_key", &sdk::g_config_system->hide_shots_key, &sdk::g_config_system->hide_shots_key_style);
			}
			ui::EndChild();
			ui::PopStyleColor(2);
			ui::PopStyleVar();
		}
		
		if (this->m_current_tab == 2) {
			ui::SetNextWindowPos(calc_pos(0));
			ui::BeginChild("player esp", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 65 + 31 - 30 - 30));
			{
				ui::Checkbox("teammates", &sdk::g_config_system->teammates);
				ui::Checkbox("dormant", &sdk::g_config_system->dormant);
				ui::Checkbox("bounding box", &sdk::g_config_system->bounding_box);
				color_picker("##bounding_box_color", sdk::g_config_system->bounding_box_color, false);
				ui::Checkbox("health bar", &sdk::g_config_system->health_bar);
				ui::Checkbox("name", &sdk::g_config_system->name);
				color_picker("##name_color", sdk::g_config_system->name_color);
				ui::Checkbox("flags", &sdk::g_config_system->flags);
				ui::Checkbox("weapon text", &sdk::g_config_system->weapon_text);
				ui::Checkbox("weapon icon", &sdk::g_config_system->weapon_icon);
				color_picker("##weapon_color", sdk::g_config_system->weapon_color);
				ui::Checkbox("ammo", &sdk::g_config_system->ammo);
				color_picker("##ammo_color", sdk::g_config_system->ammo_color);
				ui::Checkbox("distance", &sdk::g_config_system->distance);
				ui::Checkbox("glow", &sdk::g_config_system->glow);
				color_picker("##glow_color", sdk::g_config_system->glow_color);
				ui::Checkbox("hit marker", &sdk::g_config_system->hit_marker);
				ui::Checkbox("hit marker sound", &sdk::g_config_system->hit_marker_sound);
				ui::Checkbox("visualize sounds", &sdk::g_config_system->visualize_sounds);
				color_picker("##visualize_sounds_color", sdk::g_config_system->visualize_sounds_color);
				ui::Checkbox("money", &sdk::g_config_system->money);
				ui::Checkbox("skeleton", &sdk::g_config_system->skeleton);
				color_picker("##skeleton_color", sdk::g_config_system->skeleton_color);
				ui::Checkbox("out of fov arrow", &sdk::g_config_system->out_of_fow_arrow);
				color_picker("##out_of_fow_arrow_color", sdk::g_config_system->out_of_fow_arrow_color);
			}
			ui::EndChild();

			ui::SetNextWindowPos(ImVec2(ui::GetWindowPos().x - 4 + 26 + 4 + (ui::GetWindowSize().x / 2 - 36) * 0 + 20 * 0, ui::GetWindowPos().y + (ui::GetWindowSize().y / 2 - 36) + 52 + 27 - 60 + 68 - 30));
			ui::BeginChild("colored models", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 27 + 1 + 30));
			{
				ui::Checkbox("player", &sdk::g_config_system->player);
				color_picker("##player_color", sdk::g_config_system->player_color);
				if (sdk::g_config_system->player) {
                    ui::SingleSelect("##player_model", &sdk::g_config_system->player_model, { "regular", "solid", "glow", "metallic" });
                    ui::SingleSelect("chams type", &sdk::g_config_system->player_chams_type, { "regular", "solid", "glow", "metallic", "wireframe", "flat", "animated", "glow + pulse" });
                }

				ui::Checkbox("player behind wall", &sdk::g_config_system->player_behind_wall);
				color_picker("##player_behind_wall_color", sdk::g_config_system->player_behind_wall_color);
				if (sdk::g_config_system->player_behind_wall)
					ui::SingleSelect("##player_behind_wall_model", &sdk::g_config_system->player_behind_wall_model, { "regular", "solid", "glow", "metallic" });

				ui::Checkbox("teammate", &sdk::g_config_system->teammate);
				color_picker("##teammate_color", sdk::g_config_system->teammate_color);
				if (sdk::g_config_system->teammate)
					ui::SingleSelect("##teammate_model", &sdk::g_config_system->teammate_model, { "regular", "solid", "glow", "metallic" });

				ui::Checkbox("teammate behind wall", &sdk::g_config_system->teammate_behind_wall);
				color_picker("##teammate_behind_wall_color", sdk::g_config_system->teammate_behind_wall_color);
				if (sdk::g_config_system->teammate_behind_wall)
					ui::SingleSelect("##teammate_behind_wall_model", &sdk::g_config_system->teammate_behind_wall_model, { "regular", "solid", "glow", "metallic" });

				ui::Checkbox("local player", &sdk::g_config_system->local_player);
				color_picker("##local_player_color", sdk::g_config_system->local_player_color);
				if (sdk::g_config_system->local_player)
					ui::SingleSelect("##local_player_model", &sdk::g_config_system->local_player_model, { "regular", "solid", "glow", "metallic" });

				ui::Checkbox("local player fake", &sdk::g_config_system->local_player_fake);
				color_picker("##local_player_fake_color", sdk::g_config_system->local_player_fake_color);
				if (sdk::g_config_system->local_player_fake)
					ui::SingleSelect("##local_player_fake_model", &sdk::g_config_system->local_player_fake_model, { "regular", "solid", "glow", "metallic" });
								
				ui::Checkbox("on shot (random crashes)", &sdk::g_config_system->on_shot);
				color_picker("##on_shot_color", sdk::g_config_system->on_shot_color);
				if (sdk::g_config_system->on_shot) {
					ui::SingleSelect("##on_shot_model", &sdk::g_config_system->on_shot_model, { "regular", "solid", "glow", "metallic" });
					ui::SliderInt("##on_shot_time", &sdk::g_config_system->on_shot_time, 0, 5, "%ds");
				}
				ui::Checkbox("ragdolls", &sdk::g_config_system->ragdolls);

				ui::Checkbox("hands", &sdk::g_config_system->hands);
				color_picker("##hands_color", sdk::g_config_system->hands_color);
				if (sdk::g_config_system->hands)
					ui::SingleSelect("##hands_model", &sdk::g_config_system->hands_model, { "regular", "solid", "glow", "metallic" });

				ui::Checkbox("weapon viewmodel", &sdk::g_config_system->weapon_viewmodel);
				color_picker("##weapon_viewmodel_color", sdk::g_config_system->weapon_viewmodel_color);
				if (sdk::g_config_system->weapon_viewmodel_color)
					ui::SingleSelect("##weapon_viewmodel_model", &sdk::g_config_system->weapon_viewmodel_model, { "regular", "solid", "glow", "metallic" });

				ui::Checkbox("weapons", &sdk::g_config_system->weapons);
				color_picker("##weapons_color", sdk::g_config_system->weapons_color);
				if (sdk::g_config_system->weapons)
					ui::SingleSelect("##weapons_model", &sdk::g_config_system->weapons_model, { "regular", "solid", "glow", "metallic" });
				ui::Checkbox("disable model occlusion", &sdk::g_config_system->disable_model_occlusion);
				ui::Checkbox("shadow", &sdk::g_config_system->shadow);
				color_picker("##shadow_color", sdk::g_config_system->shadow_color);
				if (sdk::g_config_system->shadow)
					ui::SingleSelect("##shadow_model", &sdk::g_config_system->shadow_model, { "regular", "solid", "glow", "metallic" });
			}
			ui::EndChild();

			ui::SetNextWindowPos(calc_pos(1));
			ui::BeginChild("other esp", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 65 + 31 - 30));
			{
				ui::Checkbox("radar", &sdk::g_config_system->radar);
				ui::SingleSelect("dropped weapons", &sdk::g_config_system->dropped_weapons, { "none", "text", "icon" });
				color_picker("dropped weapons color", sdk::g_config_system->dropped_weapons_color);
				ui::Checkbox("grenades", &sdk::g_config_system->grenades);
				color_picker("##grenades_color", sdk::g_config_system->grenades_color);
				ui::Checkbox("inaccuracy overlay", &sdk::g_config_system->inaccuracy_overlay);
				color_picker("##inaccuracy_overlay_color", sdk::g_config_system->inaccuracy_overlay_color);
				ui::Checkbox("recoil overlay", &sdk::g_config_system->recoil_overlay);
				ui::Checkbox("crosshair", &sdk::g_config_system->crosshair);
				ui::Checkbox("bomb", &sdk::g_config_system->bomb);
				color_picker("##bomb_color", sdk::g_config_system->bomb_color);
				ui::Checkbox("grenade trajectory", &sdk::g_config_system->grenade_trajectory);
				color_picker("##grenade_trajectory_color", sdk::g_config_system->grenade_trajectory_color);
				ui::Checkbox("grenade proximity warning", &sdk::g_config_system->grenade_proximity_warning);
				ui::Checkbox("spectators", &sdk::g_config_system->spectators);
				ui::Checkbox("penetration reticle", &sdk::g_config_system->penetration_reticle);
				ui::MultiSelect("feature indicators", &sdk::g_config_system->feature_indicators, { "lag comp", "double tap", "hide shots", "force body aim", "force safe point", "override minimum damage", "peek assistance", "manual anti-aimbot angles"});
				if (sdk::g_config_system->feature_indicators[7]) {
					ui::Text("manual anti-aimbot angles color");
					color_picker("##manual_anti_aimbot_angles_color", sdk::g_config_system->manual_anti_aimbot_angles_color);
				}
			}
			ui::EndChild();

			ui::SetNextWindowPos(ImVec2(ui::GetWindowPos().x - 4 + 26 + (ui::GetWindowSize().x / 2 - 36) * 1 + 20 * 1, ui::GetWindowPos().y + (ui::GetWindowSize().y / 2 - 36) + 52 + 27 - 60 + 68));
			ui::BeginChild("effects", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 27 + 1));
			{
				ui::Checkbox("remove flashbang effects", &sdk::g_config_system->remove_flashbang_effects);
				ui::Checkbox("remove smoke grenades", &sdk::g_config_system->remove_smoke_grenades);
				ui::Checkbox("remove fog", &sdk::g_config_system->remove_fog);
				ui::Checkbox("remove skybox", &sdk::g_config_system->remove_skybox);				
				ui::SingleSelect("visual recoil adjustment", &sdk::g_config_system->visual_recoil_adjustment, { "off", "remove shake", "remove all" });
				ui::Text("world color");
				color_picker("##world_color", sdk::g_config_system->world_color);
				ui::Text("props color");
				color_picker("##props_color", sdk::g_config_system->props_color);
				if (!sdk::g_config_system->remove_skybox) {
					ui::Text("skybox color");
					color_picker("##skybox_color", sdk::g_config_system->skybox_color);
				}
				ui::Checkbox("full bright", &sdk::g_config_system->full_bright);
				ui::Checkbox("remove scope overlay", &sdk::g_config_system->remove_scope_overlay);
				ui::Checkbox("disable post processing", &sdk::g_config_system->disable_post_processing);
				ui::Checkbox("force third person (alive)", &sdk::g_config_system->force_third_person_alive);
				ui::Keybind("##third_person_key", &sdk::g_config_system->third_person_key, &sdk::g_config_system->third_person_key_style);
				if (sdk::g_config_system->force_third_person_alive)
					ui::SliderInt("##third_person_distance", &sdk::g_config_system->third_person_distance, 30, 300, "%d");
				ui::Checkbox("force third person (dead)", &sdk::g_config_system->force_third_person_dead);
				ui::Checkbox("disable rendering of teammates", &sdk::g_config_system->disable_rendering_of_teammates);
				ui::Checkbox("disable rendering of ragdolls", &sdk::g_config_system->disable_rendering_of_ragdolls);
				ui::Checkbox("bullet tracers", &sdk::g_config_system->bullet_tracers);
				color_picker("##bullet_tracers_color", sdk::g_config_system->bullet_tracers_color);
				ui::Checkbox("bullet impacts", &sdk::g_config_system->bullet_impacts);
			}
			ui::EndChild();
		}

		if (this->m_current_tab == 3) {
			ui::SetNextWindowPos(ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 2 - 36) * 0 + 20 * 0, ui::GetWindowPos().y + 52 + 27));
			ui::BeginChild("settings", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 65 + 31 - 30 - 30 - 120 + 24));
			{
				ui::Text("menu key");
				ui::Keybind("##menu_key", &sdk::g_config_system->menu_key);

				ui::Text("menu color");
				if (color_picker("##menu_color", sdk::g_config_system->menu_color, false)) {
					settings::m_menu_color[0] = sdk::g_config_system->menu_color[0];
					settings::m_menu_color[1] = sdk::g_config_system->menu_color[1];
					settings::m_menu_color[2] = sdk::g_config_system->menu_color[2];
					settings::m_menu_color[3] = sdk::g_config_system->menu_color[3];
				}

				ui::Checkbox("watermark", &sdk::g_config_system->menu_watermark);
			}
			ui::EndChild();
			ui::SetNextWindowPos(ImVec2(ui::GetWindowPos().x + 26 + (ui::GetWindowSize().x / 2 - 36) * 0 + 20 * 0, ui::GetWindowPos().y + (ui::GetWindowSize().y / 2 - 36) + 52 + 27 + 95 - 236 + 24 - 3));
			ui::BeginChild("movement", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 65 + 31 - 30 - 45 - 8 - 4 + 147));
			{
				ui::Checkbox("bunny hop", &sdk::g_config_system->bunny_hop);
				ui::Checkbox("air strafe", &sdk::g_config_system->air_strafe);
				ui::Checkbox("slide walk", &sdk::g_config_system->slide_walk);
				ui::Text("slow motion");
				ui::Keybind("##slow_motion_key", &sdk::g_config_system->slow_motion_key, &sdk::g_config_system->slow_motion_key_style);
				ui::SliderInt("speed limit", &sdk::g_config_system->speed_limit, 0, 260, sdk::g_config_system->speed_limit == 0 ? "auto" : "%du/s");
				ui::Text("peek assistance");
				ui::Keybind("##peek_assistence_key", &sdk::g_config_system->peek_assistence_key, &sdk::g_config_system->peek_assistence_key_style);
				if (sdk::g_config_system->peek_assistence_key) {
					ui::Text("color");
					color_picker("##peek_assistence_color", sdk::g_config_system->peek_assistence_color, false);
				}
			}
			ui::EndChild();

			ui::SetNextWindowPos(calc_pos(1));
			ui::BeginChild("others", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 + 27 - 94 + 70 + 37));
			{
				ui::Checkbox("override fov", &sdk::g_config_system->override_fov);
				ui::SliderInt("##override_fov_amount", &sdk::g_config_system->override_fov_amount, 60, 140, "%d�%");
				ui::Checkbox("override fov when scoped", &sdk::g_config_system->override_fov_when_scoped);
				ui::Checkbox("override viewmodel fov", &sdk::g_config_system->override_viewmodel_fov);
				ui::SliderInt("##override_viewmodel_fov_amount", &sdk::g_config_system->override_viewmodel_fov_amount, 60, 140, "%d�%");
				ui::Checkbox("aspect ratio", &sdk::g_config_system->aspect_ratio);
				if (sdk::g_config_system->aspect_ratio) {
					ui::SliderInt("width", &sdk::g_config_system->width, 1, 50, "%d");
					ui::SliderInt("height", &sdk::g_config_system->height, 1, 50, "%d");
				}
				ui::Checkbox("clan tag spammer", &sdk::g_config_system->clan_tag_spammer);
				ui::Checkbox("chat spammer", &sdk::g_config_system->chat_spammer);
				ui::Checkbox("name spammer", &sdk::g_config_system->name_spammer);
				ui::Checkbox("share esp", &sdk::g_config_system->share_esp);
				ui::Checkbox("log weapon purchases", &sdk::g_config_system->log_weapon_purchases);
				ui::Checkbox("log damage dealt", &sdk::g_config_system->log_damage_dealt);
				ui::SingleSelect("persistent kill feed", &sdk::g_config_system->persistent_kill_feed, { "none", "default", "only headshots" });
				ui::Text("auto buy items");
				ui::MultiSelect("##primary_weapons", &sdk::g_config_system->primary_weapons, { "nova", "xm1014", "sawed-off/mag-7", "m249", "negev", "mac-10/mp9", "mp7/mp5", "ump-45", "p90", "bizon", "galil/famas", "ak-47/m4a1-s/m4a4", "ssg 08", "sg 553/aug", "awp", "g3sg1/scar-20" });
				ui::MultiSelect("##secondary_weapons", &sdk::g_config_system->secondary_weapons, { "dual berettas", "p250", "tec-9/five-seven/cz75", "deagle/revolver" });
				ui::MultiSelect("##equipment", &sdk::g_config_system->equipment, { "kevlar", "helmet", "taser", "defuser", "fire", "decoy", "flash", "flash 2", "he", "smoke" });
				ui::SliderInt("auto buy minimal money", &sdk::g_config_system->auto_buy_minimal_money, 50, 16000, "%d$");
			}
			ui::EndChild();

			ui::SetNextWindowPos(ImVec2(ui::GetWindowPos().x - 4 + 26 + (ui::GetWindowSize().x / 2 - 36) * 1 + 20 * 1, ui::GetWindowPos().y + (ui::GetWindowSize().y / 2 - 36) + 52 + 27 - 60 + 68 + 70 - 3 - 24 + 37));
			ui::BeginChild("extra", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y / 2 - 80 - 27 + 27 + 1 - 50 - 12 - 5 + 24 - 37));
			{
				ui::Checkbox("infinite duck", &sdk::g_config_system->infinite_duck);
				ui::Text("fake duck");
				ui::Keybind("##fake_duck_key", &sdk::g_config_system->fake_duck_key, &sdk::g_config_system->fake_duck_key_style);
				ui::Text("radio");
				ui::Keybind("##radio_key", &sdk::g_config_system->radio_key, &sdk::g_config_system->radio_key_style);
				if (sdk::g_config_system->radio_key) {
					ui::SliderInt("volume", &sdk::g_config_system->volume, 0, 100, "%d%%");
					ui::SingleSelect("channel", &sdk::g_config_system->channel, { "phonk", "chanson", "podrubasik", "club", "house", "8-bit", "lo-fi", "eurobeat", "news"});
				}
			}
			ui::EndChild();
		}

		if (this->m_current_tab == 4) {
			sdk::g_config_system->menu();

			ui::SetNextWindowPos(ImVec2(ui::GetWindowPos().x - 4 + 26 + (ui::GetWindowSize().x / 2 - 36) * 1 + 20 * 1, ui::GetWindowPos().y + 52 + 27));
			ui::BeginChild("scripts", ImVec2(ui::GetWindowSize().x / 2 - 36 + 4, ui::GetWindowSize().y - 80 - 27));
			{
				ui::Text("soon...");
			}
			ui::EndChild();
		}

		if (this->m_current_tab == 5) {
			//skins::get().menu();

			//ui::EndChild();
		}

		ui::PopStyleVar();
		ui::PopStyleColor();
		ui::End();
	}

	void c_menu::draw_end()	{
		if (!this->m_initialized)
			return;

		ui::EndFrame();
		ui::Render();
		ImGui_ImplDX9_RenderDrawData(ui::GetDrawData());
	}

	void c_menu::draw_aimware_v5() {
		const auto& screen_size = ui::GetIO().DisplaySize;

		ui::SetNextWindowSizeConstraints(ImVec2(600, 650), ImVec2(screen_size.x - 2, screen_size.y - 2));
		
		const ImColor aimware_bg(11, 11, 11, 255);
		const ImColor aimware_accent(0, 212, 255, 255);
		const ImColor aimware_accent_dark(0, 150, 200, 255);
		const ImColor aimware_accent_light(50, 230, 255, 255);
		const ImColor aimware_border(28, 28, 28, 255);
		const ImColor aimware_child_bg(16, 16, 16, 255);
		const ImColor aimware_text(225, 225, 225, 255);
		const ImColor aimware_text_dim(160, 160, 160, 255);
		
		ui::PushStyleColor(ImGuiCol_Border, ImVec4(aimware_accent.r / 255.f * 0.3f, aimware_accent.g / 255.f * 0.3f, aimware_accent.b / 255.f * 0.3f, 1.f));
		ui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.5f);
		ui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.f);
		
		ui::Begin("supremacy - aimware v5 style", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MainWindow);
		
		auto* window = ui::GetCurrentWindow();
		if (window && window->DrawList) {
			const auto& pos = window->Pos;
			const auto& size = window->Size;
			
			window->DrawList->AddRectFilledMultiColor(
				ImVec2(pos.x - 1, pos.y - 1), ImVec2(pos.x + size.x + 1, pos.y + size.y + 1),
				aimware_bg, ImColor(13, 13, 13, 255),
				ImColor(13, 13, 13, 255), aimware_bg
			);
			
			window->DrawList->AddRect(
				ImVec2(pos.x - 1, pos.y - 1), ImVec2(pos.x + size.x + 1, pos.y + size.y + 1),
				aimware_accent, 8.f, 0, 2.5f
			);
			
			window->DrawList->AddRect(
				ImVec2(pos.x - 2, pos.y - 2), ImVec2(pos.x + size.x + 2, pos.y + size.y + 2),
				ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 30), 8.f, 0, 1.f
			);
			
			window->DrawList->AddRectFilled(
				ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + 40),
				ImColor(18, 18, 18, 255)
			);
			
			window->DrawList->AddRectFilledMultiColor(
				ImVec2(pos.x, pos.y + 40), ImVec2(pos.x + size.x, pos.y + 42),
				aimware_accent, aimware_accent_dark,
				aimware_accent_dark, aimware_accent
			);
			
			window->DrawList->AddRectFilledMultiColor(
				ImVec2(pos.x, pos.y + 42), ImVec2(pos.x + size.x, pos.y + 44),
				ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 20),
				ImColor(aimware_accent_dark.r, aimware_accent_dark.g, aimware_accent_dark.b, 20),
				ImColor(aimware_accent_dark.r, aimware_accent_dark.g, aimware_accent_dark.b, 20),
				ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 20)
			);
		}
		
		ui::SetCursorPos(ImVec2(15, 12));
		ui::PushStyleColor(ImGuiCol_Text, aimware_accent_light);
		ui::PushFont(ui::GetIO().Fonts->Fonts[1]);
		ui::Text("SUPREMACY");
		ui::PopFont();
		ui::PopStyleColor();
		
		ui::SetCursorPos(ImVec2(15, 50));
		
		ui::TabButtonAimware("AIMBOT", &this->m_current_tab, 0, 6, aimware_accent);
		ui::TabButtonAimware("ANTI-AIM", &this->m_current_tab, 1, 6, aimware_accent);
		ui::TabButtonAimware("VISUALS", &this->m_current_tab, 2, 6, aimware_accent);
		ui::TabButtonAimware("MISC", &this->m_current_tab, 3, 6, aimware_accent);
		ui::TabButtonAimware("CONFIG", &this->m_current_tab, 4, 6, aimware_accent);
		ui::TabButtonAimware("SKINS", &this->m_current_tab, 5, 6, aimware_accent);
		
		ui::SetCursorPos(ImVec2(ui::GetWindowWidth() - 120, 12));
		ui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.12f, 0.12f, 0.9f));
		ui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.18f, 0.18f, 0.18f, 0.95f));
		ui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.22f, 0.22f, 0.22f, 1.f));
		ui::PushStyleColor(ImGuiCol_Text, aimware_text);
		ui::PushStyleColor(ImGuiCol_Border, ImVec4(aimware_accent.r / 255.f * 0.3f, aimware_accent.g / 255.f * 0.3f, aimware_accent.b / 255.f * 0.3f, 1.f));
		ui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
		ui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
		if (ui::Button("Switch Style", ImVec2(100, 25))) {
			m_aimware_style = false;
		}
		ui::PopStyleVar(2);
		ui::PopStyleColor(5);
		
		ui::SetCursorPos(ImVec2(15, 90));
		
		const auto child_size = ImVec2(ui::GetWindowSize().x / 2 - 40, ui::GetWindowSize().y - 110);
		static auto calc_pos_aw = [](int num) -> ImVec2 {
			if (num == 1)
				return ImVec2(ui::GetWindowPos().x + 20 + (ui::GetWindowSize().x / 2 - 20) * num + 10 * num, ui::GetWindowPos().y + 100);
			else
				return ImVec2(ui::GetWindowPos().x + 20 + (ui::GetWindowSize().x / 2 - 20) * num + 10 * num, ui::GetWindowPos().y + 100);
		};
		
		if (this->m_current_tab == 0) {
			auto weapon_index = clamp_weapon_index(sdk::g_config_system->weapon_selection);
			
			ui::SetNextWindowPos(calc_pos_aw(0));
			ui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
			ui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.f);
			ui::PushStyleColor(ImGuiCol_ChildBg, aimware_child_bg);
			ui::PushStyleColor(ImGuiCol_Border, ImColor(aimware_border.r, aimware_border.g, aimware_border.b, 180));
			ui::BeginChild("aimbot_aw", child_size, true);
			{
				auto* child_window = ui::GetCurrentWindow();
				if (child_window && child_window->DrawList) {
					const auto& pos = child_window->Pos;
					const auto& size = child_window->Size;
					child_window->DrawList->AddRect(
						pos, ImVec2(pos.x + size.x, pos.y + size.y),
						ImColor(aimware_accent.r * 0.25f, aimware_accent.g * 0.25f, aimware_accent.b * 0.25f, 255),
						6.f, 0, 1.5f
					);
					child_window->DrawList->AddRectFilledMultiColor(
						ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + 1),
						ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 30),
						ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 30),
						ImColor(aimware_accent.r * 0.5f, aimware_accent.g * 0.5f, aimware_accent.b * 0.5f, 30),
						ImColor(aimware_accent.r * 0.5f, aimware_accent.g * 0.5f, aimware_accent.b * 0.5f, 30)
					);
				}
				ui::Checkbox("enabled", &sdk::g_config_system->enabled);
				ui::Keybind("##enabled_key", &sdk::g_config_system->enabled_key, &sdk::g_config_system->enabled_key_style);
				
				sdk::g_config_system->weapon_selection = weapon_index;
				ui::SingleSelect("weapon selection", &sdk::g_config_system->weapon_selection, k_weapon_names);
				weapon_index = clamp_weapon_index(sdk::g_config_system->weapon_selection);
				
				draw_weapon_aim_section(weapon_index);
				ui::Checkbox("log misses due to spread", &sdk::g_config_system->log_misses_due_to_spread);
			}
			ui::EndChild();
			ui::PopStyleColor(2);
			ui::PopStyleVar(2);
			
			ui::SetNextWindowPos(calc_pos_aw(1));
			ui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
			ui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.f);
			ui::PushStyleColor(ImGuiCol_ChildBg, aimware_child_bg);
			ui::PushStyleColor(ImGuiCol_Border, ImColor(aimware_border.r, aimware_border.g, aimware_border.b, 180));
			ui::BeginChild("other_aw", child_size, true);
			{
				auto* child_window = ui::GetCurrentWindow();
				if (child_window && child_window->DrawList) {
					const auto& pos = child_window->Pos;
					const auto& size = child_window->Size;
					child_window->DrawList->AddRect(
						pos, ImVec2(pos.x + size.x, pos.y + size.y),
						ImColor(aimware_accent.r * 0.25f, aimware_accent.g * 0.25f, aimware_accent.b * 0.25f, 255),
						6.f, 0, 1.5f
					);
					child_window->DrawList->AddRectFilledMultiColor(
						ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + 1),
						ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 30),
						ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 30),
						ImColor(aimware_accent.r * 0.5f, aimware_accent.g * 0.5f, aimware_accent.b * 0.5f, 30),
						ImColor(aimware_accent.r * 0.5f, aimware_accent.g * 0.5f, aimware_accent.b * 0.5f, 30)
					);
				}
				draw_weapon_other_section(weapon_index);
			}
			ui::EndChild();
			ui::PopStyleColor(2);
			ui::PopStyleVar(2);
		}
		
		if (this->m_current_tab == 1) {
			ui::SetNextWindowPos(calc_pos_aw(0));
			ui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
			ui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.f);
			ui::PushStyleColor(ImGuiCol_ChildBg, aimware_child_bg);
			ui::PushStyleColor(ImGuiCol_Border, ImColor(aimware_border.r, aimware_border.g, aimware_border.b, 180));
			ui::BeginChild("anti-aim_aw", child_size, true);
			{
				auto* child_window = ui::GetCurrentWindow();
				if (child_window && child_window->DrawList) {
					const auto& pos = child_window->Pos;
					const auto& size = child_window->Size;
					child_window->DrawList->AddRect(
						pos, ImVec2(pos.x + size.x, pos.y + size.y),
						ImColor(aimware_accent.r * 0.25f, aimware_accent.g * 0.25f, aimware_accent.b * 0.25f, 255),
						6.f, 0, 1.5f
					);
					child_window->DrawList->AddRectFilledMultiColor(
						ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + 1),
						ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 30),
						ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 30),
						ImColor(aimware_accent.r * 0.5f, aimware_accent.g * 0.5f, aimware_accent.b * 0.5f, 30),
						ImColor(aimware_accent.r * 0.5f, aimware_accent.g * 0.5f, aimware_accent.b * 0.5f, 30)
					);
				}
				ui::Checkbox("enabled", &sdk::g_config_system->enabled2);
				ui::SliderInt("pitch", &sdk::g_config_system->pitch, -89, 89, "%d°");
				ui::SingleSelect("yaw base", &sdk::g_config_system->yaw_base, { "local view", "closest", "average", "fov" });
				ui::SliderInt("yaw", &sdk::g_config_system->yaw, -180, 180, "%d°");
				ui::Checkbox("freestanding", &sdk::g_config_system->freestanding);
				ui::Keybind("##freestanding_key", &sdk::g_config_system->freestanding_key, &sdk::g_config_system->freestanding_key_style);
				ui::SliderInt("jitter range", &sdk::g_config_system->jitter_range, 0, 180, "%d°");
				ui::SingleSelect("jitter switch", &sdk::g_config_system->jitter_switch, { "sending packet", "random" });
				ui::SliderInt("rotate range", &sdk::g_config_system->rotate_range, 0, 360, "%d°");
				ui::SliderInt("rotate speed", &sdk::g_config_system->rotate_speed, 0, 100, "%d%%");
				ui::SingleSelect("body yaw", &sdk::g_config_system->body_yaw, { "off", "static", "jitter" });
				ui::Keybind("##body_yaw_key", &sdk::g_config_system->body_yaw_key, &sdk::g_config_system->body_yaw_key_style);
				if (sdk::g_config_system->body_yaw) {
					ui::SliderInt("yaw limit", &sdk::g_config_system->yaw_limit, 0, 58, "%d°");
					ui::SliderInt("body lean", &sdk::g_config_system->body_lean, -180, 180, "%d°");
					ui::SliderInt("inverted body lean", &sdk::g_config_system->inverted_body_lean, -180, 180, "%d°");
					ui::SingleSelect("on shot side", &sdk::g_config_system->on_shot_side, { "off", "left", "right", "opposite", "switch" });
					if (sdk::g_config_system->body_yaw == 1)
						ui::SingleSelect("automatic side selection", &sdk::g_config_system->automatic_side_selection, { "off", "peek fake", "peek real" });
				}
				ui::Spacing();
				ui::Checkbox("custom desync", &sdk::g_config_system->custom_desync);
				if (sdk::g_config_system->custom_desync) {
					ui::SingleSelect("desync mode", &sdk::g_config_system->desync_mode, { "default", "jitter", "static", "random" });
					ui::SliderFloat("desync amount", &sdk::g_config_system->desync_amount, 0.f, 120.f, "%.1f°");
					if (sdk::g_config_system->desync_mode == 3)
						ui::SliderFloat("desync inverter", &sdk::g_config_system->desync_inverter, -1.f, 1.f, "%.2f");
				}
				ui::Spacing();
				const char* fake_peek_label_aw = sdk::g_config_system->fake_peek_mode == 0 ? "fake peek" : "fake head";
				ui::Checkbox(fake_peek_label_aw, &sdk::g_config_system->fake_peek);
				if (sdk::g_config_system->fake_peek) {
					ui::SingleSelect("name", &sdk::g_config_system->fake_peek_mode, { "fake peek", "fake head" });
					ui::SingleSelect("mode", &sdk::g_config_system->fake_peek_type, { "normal", "break" });
					ui::Keybind("##fake_peek_key_aw", &sdk::g_config_system->fake_peek_key, &sdk::g_config_system->fake_peek_key_style);
				}
				ui::Spacing();
				ui::Text("manual left");
				ui::Keybind("##manual_left_key", &sdk::g_config_system->manual_left_key);
				ui::Text("manual right");
				ui::Keybind("##manual_right_key", &sdk::g_config_system->manual_right_key);
				ui::Text("manual back");
				ui::Keybind("##manual_back_key", &sdk::g_config_system->manual_back_key);
			}
			ui::EndChild();
			ui::PopStyleColor(2);
			ui::PopStyleVar(2);
		}
		
		if (this->m_current_tab == 2) {
			ui::SetNextWindowPos(calc_pos_aw(0));
			ui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
			ui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.f);
			ui::PushStyleColor(ImGuiCol_ChildBg, aimware_child_bg);
			ui::PushStyleColor(ImGuiCol_Border, ImColor(aimware_border.r, aimware_border.g, aimware_border.b, 180));
			ui::BeginChild("visuals_aw", child_size, true);
			{
				auto* child_window = ui::GetCurrentWindow();
				if (child_window && child_window->DrawList) {
					const auto& pos = child_window->Pos;
					const auto& size = child_window->Size;
					child_window->DrawList->AddRect(
						pos, ImVec2(pos.x + size.x, pos.y + size.y),
						ImColor(aimware_accent.r * 0.25f, aimware_accent.g * 0.25f, aimware_accent.b * 0.25f, 255),
						6.f, 0, 1.5f
					);
					child_window->DrawList->AddRectFilledMultiColor(
						ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + 1),
						ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 30),
						ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 30),
						ImColor(aimware_accent.r * 0.5f, aimware_accent.g * 0.5f, aimware_accent.b * 0.5f, 30),
						ImColor(aimware_accent.r * 0.5f, aimware_accent.g * 0.5f, aimware_accent.b * 0.5f, 30)
					);
				}
				ui::Checkbox("teammates", &sdk::g_config_system->teammates);
				ui::Checkbox("dormant", &sdk::g_config_system->dormant);
				ui::Checkbox("bounding box", &sdk::g_config_system->bounding_box);
				color_picker("bounding box color", sdk::g_config_system->bounding_box_color);
				ui::Checkbox("health bar", &sdk::g_config_system->health_bar);
				ui::Checkbox("name", &sdk::g_config_system->name);
				color_picker("name color", sdk::g_config_system->name_color);
				ui::Checkbox("flags", &sdk::g_config_system->flags);
				ui::Checkbox("weapon text", &sdk::g_config_system->weapon_text);
				ui::Checkbox("weapon icon", &sdk::g_config_system->weapon_icon);
				color_picker("weapon color", sdk::g_config_system->weapon_color);
				ui::Checkbox("ammo", &sdk::g_config_system->ammo);
				color_picker("ammo color", sdk::g_config_system->ammo_color);
				ui::Checkbox("distance", &sdk::g_config_system->distance);
				ui::Checkbox("money", &sdk::g_config_system->money);
				ui::Checkbox("skeleton", &sdk::g_config_system->skeleton);
				color_picker("skeleton color", sdk::g_config_system->skeleton_color);
			}
			ui::EndChild();
			ui::PopStyleColor(2);
			ui::PopStyleVar(2);
		}
		
		if (this->m_current_tab == 3) {
			ui::SetNextWindowPos(calc_pos_aw(0));
			ui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
			ui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.f);
			ui::PushStyleColor(ImGuiCol_ChildBg, aimware_child_bg);
			ui::PushStyleColor(ImGuiCol_Border, ImColor(aimware_border.r, aimware_border.g, aimware_border.b, 180));
			ui::BeginChild("misc_aw", child_size, true);
			{
				auto* child_window = ui::GetCurrentWindow();
				if (child_window && child_window->DrawList) {
					const auto& pos = child_window->Pos;
					const auto& size = child_window->Size;
					child_window->DrawList->AddRect(
						pos, ImVec2(pos.x + size.x, pos.y + size.y),
						ImColor(aimware_accent.r * 0.25f, aimware_accent.g * 0.25f, aimware_accent.b * 0.25f, 255),
						6.f, 0, 1.5f
					);
					child_window->DrawList->AddRectFilledMultiColor(
						ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + 1),
						ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 30),
						ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 30),
						ImColor(aimware_accent.r * 0.5f, aimware_accent.g * 0.5f, aimware_accent.b * 0.5f, 30),
						ImColor(aimware_accent.r * 0.5f, aimware_accent.g * 0.5f, aimware_accent.b * 0.5f, 30)
					);
				}
				ui::Checkbox("fake lag", &sdk::g_config_system->fake_lag);
				ui::SingleSelect("amount", &sdk::g_config_system->amount, { "dynamic", "maximal", "switch", "break lc" });
				ui::MultiSelect("conditions", &sdk::g_config_system->conditions, { "always", "on move", "in air" });
				ui::SliderInt("factor limit", &sdk::g_config_system->factor_limit, 0, 14, "%dticks");
				ui::Checkbox("force fake lag when peeking", &sdk::g_config_system->force_when_peeking);
				ui::Checkbox("hide shots", &sdk::g_config_system->hide_shots);
				ui::Keybind("##hide_shots_key", &sdk::g_config_system->hide_shots_key, &sdk::g_config_system->hide_shots_key_style);
			}
			ui::EndChild();
			ui::PopStyleColor(2);
			ui::PopStyleVar(2);
		}
		
		if (this->m_current_tab == 4) {
			ui::SetNextWindowPos(calc_pos_aw(0));
			ui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
			ui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.f);
			ui::PushStyleColor(ImGuiCol_ChildBg, aimware_child_bg);
			ui::PushStyleColor(ImGuiCol_Border, ImColor(aimware_border.r, aimware_border.g, aimware_border.b, 180));
			ui::BeginChild("config_aw", child_size, true);
			{
				auto* child_window = ui::GetCurrentWindow();
				if (child_window && child_window->DrawList) {
					const auto& pos = child_window->Pos;
					const auto& size = child_window->Size;
					child_window->DrawList->AddRect(
						pos, ImVec2(pos.x + size.x, pos.y + size.y),
						ImColor(aimware_accent.r * 0.25f, aimware_accent.g * 0.25f, aimware_accent.b * 0.25f, 255),
						6.f, 0, 1.5f
					);
					child_window->DrawList->AddRectFilledMultiColor(
						ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + 1),
						ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 30),
						ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 30),
						ImColor(aimware_accent.r * 0.5f, aimware_accent.g * 0.5f, aimware_accent.b * 0.5f, 30),
						ImColor(aimware_accent.r * 0.5f, aimware_accent.g * 0.5f, aimware_accent.b * 0.5f, 30)
					);
				}
				sdk::g_config_system->menu();
			}
			ui::EndChild();
			ui::PopStyleColor(2);
			ui::PopStyleVar(2);
		}
		
		if (this->m_current_tab == 5) {
			ui::SetNextWindowPos(calc_pos_aw(0));
			ui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
			ui::PushStyleVar(ImGuiStyleVar_ChildRounding, 6.f);
			ui::PushStyleColor(ImGuiCol_ChildBg, aimware_child_bg);
			ui::PushStyleColor(ImGuiCol_Border, ImColor(aimware_border.r, aimware_border.g, aimware_border.b, 180));
			ui::BeginChild("skins_aw", child_size, true);
			{
				auto* child_window = ui::GetCurrentWindow();
				if (child_window && child_window->DrawList) {
					const auto& pos = child_window->Pos;
					const auto& size = child_window->Size;
					child_window->DrawList->AddRect(
						pos, ImVec2(pos.x + size.x, pos.y + size.y),
						ImColor(aimware_accent.r * 0.25f, aimware_accent.g * 0.25f, aimware_accent.b * 0.25f, 255),
						6.f, 0, 1.5f
					);
					child_window->DrawList->AddRectFilledMultiColor(
						ImVec2(pos.x, pos.y), ImVec2(pos.x + size.x, pos.y + 1),
						ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 30),
						ImColor(aimware_accent.r, aimware_accent.g, aimware_accent.b, 30),
						ImColor(aimware_accent.r * 0.5f, aimware_accent.g * 0.5f, aimware_accent.b * 0.5f, 30),
						ImColor(aimware_accent.r * 0.5f, aimware_accent.g * 0.5f, aimware_accent.b * 0.5f, 30)
					);
				}
				ui::Text("Skins menu - coming soon");
			}
			ui::EndChild();
			ui::PopStyleColor(2);
			ui::PopStyleVar(2);
		}
		
		ui::PopStyleVar(2);
		ui::PopStyleColor();
		ui::End();
	}

	void c_menu::handle() {
		if (!this->m_is_opened && ui::GetStyle().Alpha > 0.f) {
			float fc = 255.f / 0.2f * ui::GetIO().DeltaTime;
			ui::GetStyle().Alpha = std::clamp(ui::GetStyle().Alpha - fc / 255.f, 0.f, 1.f);
		}

		if (this->m_is_opened && ui::GetStyle().Alpha < 1.f) {
			float fc = 255.f / 0.2f * ui::GetIO().DeltaTime;
			ui::GetStyle().Alpha = std::clamp(ui::GetStyle().Alpha + fc / 255.f, 0.f, 1.f);
		}
	}

	bool c_menu::is_menu_initialized() {
		return this->m_initialized;
	}

	bool c_menu::is_menu_opened() {
		return this->m_is_opened;
	}

	void c_menu::set_menu_opened(bool v) {
		this->m_is_opened = settings::m_is_opened = v;
	}
}