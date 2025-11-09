#include "../../../supremacy.hpp"

namespace supremacy::hacks {
	void c_anti_aim::at_target(float& yaw) const {
		if (!sdk::g_config_system->yaw_base)
			return;

		valve::c_player* best_player{};
		auto best_value = std::numeric_limits< float >::max();

		float total_yaw{};
		int valid_count{};

		for (auto i = 1; i <= valve::g_global_vars->m_max_clients; ++i) {
			const auto player = static_cast<valve::c_player*>(
				valve::g_entity_list->find_entity(i)
				);
			if (!player
				|| player->dormant()
				|| !player->alive()
				|| player->friendly()
				|| player == valve::g_local_player)
				continue;

			switch (sdk::g_config_system->yaw_base) {
			case 1: {
				const auto dist = (valve::g_local_player->origin() - player->origin()).length();
				if (dist >= best_value)
					continue;

				best_value = dist;
				best_player = player;
			} break;
			case 2: {
				++valid_count;

				const auto x = player->origin().x - valve::g_local_player->origin().x;
				const auto y = player->origin().y - valve::g_local_player->origin().y;

				if (x == 0.f
					&& y == 0.f)
					continue;

				total_yaw += math::to_deg(std::atan2(y, x));
			} break;
			case 3: {
				const auto fov = math::calc_fov(g_context->view_angles(), g_context->shoot_pos(), player->world_space_center());
				if (fov >= best_value)
					continue;

				best_value = fov;
				best_player = player;
			} break;
			}
		}

		if (sdk::g_config_system->yaw_base == 2) {
			if (valid_count)
				yaw = total_yaw / valid_count;

			return;
		}

		if (!best_player)
			return;

		const auto x = best_player->origin().x - valve::g_local_player->origin().x;
		const auto y = best_player->origin().y - valve::g_local_player->origin().y;

		yaw = x == 0.f && y == 0.f ? 0.f : math::to_deg(std::atan2(y, x));
	}

	bool c_anti_aim::auto_direction(float& yaw) {
		bool should_freestand = sdk::g_config_system->freestanding && key_handler::check_key(sdk::g_config_system->freestanding_key, sdk::g_config_system->freestanding_key_style);

		if (!sdk::g_config_system->automatic_side_selection && !should_freestand)
			return false;		

		constexpr float STEP{ 3.f };
		constexpr float RANGE{ 48.f };
		const auto& local_shoot_pos = g_context->shoot_pos();

		struct auto_target_t { float fov; valve::c_player* player; float distance; };
		auto_target_t target{ 180.f + 1.f, nullptr, 0.f };

		for (auto i = 1; i <= valve::g_global_vars->m_max_clients; ++i) {
			const auto player = static_cast<valve::c_player*>(
				valve::g_entity_list->find_entity(i)
				);

			if (!player
				|| player->dormant()
				|| !player->alive()
				|| player->friendly()
				|| player == valve::g_local_player)
				continue;

			const auto fov = math::calc_fov(g_context->view_angles(), g_context->shoot_pos(), player->world_space_center());
			const auto dist = (player->origin() - valve::g_local_player->origin()).length();

			const auto score = fov + (dist / 1000.f);
			if (score < target.fov + (target.distance / 1000.f)) {
				target.fov = fov;
				target.player = player;
				target.distance = dist;
			}
		}

		if (!target.player) {
			if (should_freestand) {
				yaw = m_auto_yaw = std::remainder(yaw - 180.f, 360.f);
				return true;
			}

			return false;
		}

		std::vector< c_adaptive_angle > angles{ };
		angles.emplace_back(yaw - 180.f);
		angles.emplace_back(yaw + 90.f);
		angles.emplace_back(yaw - 90.f);
		angles.emplace_back(yaw + 45.f);
		angles.emplace_back(yaw - 45.f);

		bool valid{ false };

		auto start = target.player->origin() + target.player->view_offset();
		for (auto it = angles.begin(); it != angles.end(); ++it) {
			const auto rad_yaw = math::to_rad(it->m_yaw);
			vec3_t end{ local_shoot_pos.x + std::cos(rad_yaw) * RANGE,
				local_shoot_pos.y + std::sin(rad_yaw) * RANGE,
				local_shoot_pos.z };

			vec3_t dir = end - start;
			float len = dir.normalize();

			if (len <= 0.f)
				continue;

			for (float i{ 0.f }; i < len; i += STEP) {
				vec3_t point = start + (dir * i);

				int contents = valve::g_engine_trace->point_contents(point, valve::e_mask::shot_hull);

				if (!(contents & valve::e_mask::shot_hull))
					continue;

				float mult = 1.f;

				if (i > (len * 0.3f))
					mult = 1.15f;

				if (i > (len * 0.5f))
					mult = 1.35f;

				if (i > (len * 0.7f))
					mult = 1.6f;

				if (i > (len * 0.85f))
					mult = 2.2f;

				if (i > (len * 0.95f))
					mult = 3.f;

				it->m_dist += (STEP * mult);

				valid = true;
			}
		}

		if (!valid) {
			if (should_freestand) {
				yaw = m_auto_yaw = std::remainder(yaw - 180.f, 360.f);
				return true;
			}

			return false;
		}

		const auto dist_01 = std::abs(angles.at(0u).m_dist - angles.at(1u).m_dist);
		const auto dist_02 = std::abs(angles.at(0u).m_dist - angles.at(2u).m_dist);
		const auto dist_12 = std::abs(angles.at(1u).m_dist - angles.at(2u).m_dist);

		const float threshold = 12.f;
		
		if (dist_01 >= threshold || dist_02 >= threshold || dist_12 >= threshold) {
			std::sort(angles.begin(), angles.end(),
				[](const c_adaptive_angle& a, const c_adaptive_angle& b) {
				return a.m_dist > b.m_dist;
			});

			c_adaptive_angle* best = &angles.front();
			
			const float max_dist = target.distance < 500.f ? 350.f : 450.f;
			if (best->m_dist > max_dist)
				return false;						

			if (should_freestand)
				m_auto_yaw = std::remainder(best->m_yaw, 360.f);

			if (sdk::g_config_system->automatic_side_selection) {
				const auto diff = math::angle_diff(yaw, best->m_yaw);
				
				if (sdk::g_config_system->automatic_side_selection == 1) {
					m_auto_dir_side = (diff >= 0.f ? 1 : 2);
				} else {
					m_auto_dir_side = (diff < 0.f ? 1 : 2);
				}

				if (std::abs(sdk::g_config_system->yaw) > 90.f)
					m_auto_dir_side = (m_auto_dir_side == 1 ? 2 : 1);

				if (should_freestand)
					m_auto_dir_side = 0;
			}
		}

		if (should_freestand) {
			yaw = m_auto_yaw;
			return true;
		}

		return false;
	}

	int c_anti_aim::select_side() {
		if (!valve::g_client_state->m_choked_cmds
			&& m_prev_tick_count != valve::g_global_vars->m_tick_count)
			++m_side_counter;

		m_prev_tick_count = valve::g_global_vars->m_tick_count;

		const auto shot_cmd = g_anim_sync->local_data().m_shot_cmd_number;
		if (shot_cmd > valve::g_client_state->m_last_cmd_out
			&& shot_cmd <= (valve::g_client_state->m_last_cmd_out + valve::g_client_state->m_choked_cmds + 1)) {
			switch (sdk::g_config_system->on_shot_side) {
			case 1: return 2; break;
			case 2: return 1; break;
			case 3: {
				const auto& local_data = g_eng_pred->local_data().at(valve::g_client_state->m_last_cmd_out % 150);

				return (math::angle_diff(local_data.m_anim_state.m_eye_yaw, local_data.m_anim_state.m_foot_yaw) <= 0.f) + 1;
			} break;
			case 4: return m_on_shot_side_counter = (m_on_shot_side_counter != 2) + 1; break;
			}

			return 0;
		}

		if (sdk::g_config_system->body_yaw) {
			if (sdk::g_config_system->body_yaw == 2)
				return 2 - (m_side_counter % 2);

			if (sdk::g_config_system->automatic_side_selection
				&& (m_manual_side == 1 || m_manual_side == 2))
				m_auto_dir_side = (sdk::g_config_system->automatic_side_selection == 1 ? 1 : 2);

			if (m_auto_dir_side)
				return m_auto_dir_side;

			return key_handler::check_key(sdk::g_config_system->body_yaw_key, sdk::g_config_system->body_yaw_key_style) ? 2 : 1;
		}

		return 0;
	}

	void c_anti_aim::choke(valve::user_cmd_t* const user_cmd) {
		if (g_exploits->charged()
			|| g_context->freeze_time()
			|| g_movement->should_fake_duck()
			|| valve::g_client_state->m_choked_cmds > 14
			|| !(g_context->flags() & e_context_flags::can_choke)
			|| valve::g_local_player->flags() & valve::e_ent_flags::frozen)
			return;

		if (enabled(user_cmd)
			&& valve::g_client_state->m_choked_cmds < 1)
			g_context->flags() |= e_context_flags::choke;

		if (!sdk::g_config_system->fake_lag)
			return;

		bool should_choke{};
		if (sdk::g_config_system->conditions[0])
			should_choke = true;

		if (!should_choke
			&& sdk::g_config_system->conditions[1]
			&& valve::g_local_player->velocity().length() > 2.f)
			should_choke = true;

		if (!should_choke
			&& sdk::g_config_system->conditions[2]
			&& !(valve::g_local_player->flags() & valve::e_ent_flags::on_ground))
			should_choke = true;

		if (!should_choke			
			&& g_aim_bot->is_peeking())
			should_choke = true;

		if (should_choke
			&& m_choke_start_cmd_number != valve::g_client_state->m_last_cmd_out)
			m_choke_start_cmd_number = valve::g_client_state->m_last_cmd_out;

		if (g_context->flags() & e_context_flags::choke
			|| (!should_choke && m_choke_start_cmd_number != valve::g_client_state->m_last_cmd_out))
			return;

		switch (sdk::g_config_system->amount) {
		case 0:
		{
			int wish_ticks{};
			int adaptive_ticks{};
			bool should_choked{};
			const int units_per_tick = static_cast<int>(valve::to_time(valve::g_local_player->velocity().length()));

			if (wish_ticks * units_per_tick > 68)
				should_choked = valve::g_client_state->m_choked_cmds < wish_ticks;
			else if ((adaptive_ticks - 1) * units_per_tick > 68) {
				++wish_ticks;
				should_choked = valve::g_client_state->m_choked_cmds < wish_ticks;
			}
			else if (adaptive_ticks * units_per_tick > 68)
				should_choked = valve::g_client_state->m_choked_cmds < wish_ticks + 2;
			else if ((adaptive_ticks + 1) * units_per_tick > 68)
				should_choked = valve::g_client_state->m_choked_cmds < wish_ticks + 3;
			else {
				if ((adaptive_ticks + 2) * units_per_tick <= 68) {
					adaptive_ticks += 5;
					wish_ticks += 5;

					if (adaptive_ticks > 16) {
						should_choked = valve::g_client_state->m_choked_cmds < wish_ticks;

						break;
					}
				}

				should_choked = valve::g_client_state->m_choked_cmds < wish_ticks + 4;
			}

			if (!should_choked)
				g_context->flags() &= ~e_context_flags::choke;
			else
				g_context->flags() |= e_context_flags::choke;
		}
		break;
		case 1:
			g_context->flags() |= e_context_flags::choke;
			break;
		case 2:
			if (user_cmd->m_number % 30 >= 15)
				g_context->flags() |= e_context_flags::choke;

			break;
		case 3:
			if (g_context->broke_lc())
				g_context->flags() &= ~e_context_flags::choke;
			else
				g_context->flags() |= e_context_flags::choke;

			break;
		}

		if (valve::g_client_state->m_choked_cmds >= sdk::g_config_system->factor_limit
			&& !(sdk::g_config_system->force_when_peeking && g_aim_bot->is_peeking()))
			g_context->flags() &= ~e_context_flags::choke;
	}

	void c_anti_aim::select_yaw(float& yaw, const int side, int mode) {
		if (!valve::g_client_state->m_choked_cmds)
			m_choke_cycle_switch = !m_choke_cycle_switch;

		yaw = g_context->view_angles().y;
		m_auto_yaw = std::remainder(yaw - 180.f, 360.f);
		m_auto_dir_side = 0;

		if (m_manual_side == 1)
			yaw += 90.f;
		else if (m_manual_side == 2)
			yaw -= 90.f;
		else if (m_manual_side == 3)
			yaw += 180.f;
		else {
			at_target(yaw);
			if (!auto_direction(yaw))
				yaw += sdk::g_config_system->yaw;

			int aa_mode = mode;
			if (mode == 0)
				aa_mode = sdk::g_config_system->anti_aim_mode;
			switch (aa_mode) {
				case 1:
					{
						const auto base_jitter = sdk::g_config_system->jitter_range;
						const auto random_factor = g_context->addresses().m_random_int(0, 100);
						const auto jitter_amount = base_jitter * (random_factor / 100.f);
						yaw += (random_factor % 2 == 0 ? -1 : 1) * jitter_amount;
					}
					break;
				case 2:
					{
						const auto spiral_speed = sdk::g_config_system->spiral_speed * 15.f;
						const auto time_factor = valve::g_global_vars->m_cur_time * spiral_speed;
						const auto spiral_offset = std::fmod(time_factor, 360.f);
						yaw += spiral_offset;
					}
					break;
				case 3:
					{
						static int pulse_switch = 0;
						static float last_time = 0;
						const auto pulse_speed = (float)sdk::g_config_system->pulse_speed / 80.0f;
						if (valve::g_global_vars->m_cur_time - last_time > pulse_speed) {
							pulse_switch = !pulse_switch;
							last_time = valve::g_global_vars->m_cur_time;
						}
						const auto pulse_range = sdk::g_config_system->pulse_range;
						yaw += (pulse_switch ? pulse_range : -pulse_range);
					}
					break;
				default:
					break;
			}

			float jitter_offset = 0.f;
			if (sdk::g_config_system->jitter_switch) {
				const auto jitter_range = sdk::g_config_system->jitter_range;
				jitter_offset = g_context->addresses().m_random_int(-jitter_range, jitter_range) * 0.5f;
			} else {
				const auto jitter_range = sdk::g_config_system->jitter_range;
				jitter_offset = jitter_range * (m_choke_cycle_switch ? 0.5f : -0.5f);
			}

			float rotation_offset = 0.f;
			if (sdk::g_config_system->rotate_range && sdk::g_config_system->rotate_speed) {
				const auto rotate_speed = sdk::g_config_system->rotate_speed * 18.f;
				const auto rotate_range = sdk::g_config_system->rotate_range;
				const auto time_factor = valve::g_global_vars->m_cur_time * rotate_speed;
				rotation_offset = std::fmod(time_factor, rotate_range) - rotate_range * 0.5f;
			}

			// Body yaw offset
			float body_yaw_offset = 0.f;
			if (sdk::g_config_system->body_yaw) {
				body_yaw_offset = (side == 2 ? sdk::g_config_system->inverted_body_lean : sdk::g_config_system->body_lean);
			}

			yaw += jitter_offset + rotation_offset + body_yaw_offset;
		}
	}

	void c_anti_aim::set_pitch(valve::user_cmd_t& user_cmd) {
		const auto anim_state = valve::g_local_player->anim_state();
		if (!anim_state
			|| !enabled(&user_cmd)
			|| (g_context->flags() & e_context_flags::can_shoot
				&& g_context->will_shoot(valve::g_local_player->weapon(), user_cmd)))
			return;

		user_cmd.m_view_angles.x = sdk::g_config_system->pitch;
	}

	void c_anti_aim::process(valve::user_cmd_t& user_cmd, const float yaw, const int side, const int choked_cmds) const {
		const auto anim_state = valve::g_local_player->anim_state();
		if (!anim_state
			|| !enabled(&user_cmd))
			return;

		const auto shot_cmd_number = g_anim_sync->local_data().m_shot_cmd_number;
		if (user_cmd.m_number == shot_cmd_number)
			return;

		if (!side) {
			user_cmd.m_view_angles.y = std::remainder(yaw, 360.f);

			return;
		}

		const auto in_shot = shot_cmd_number > valve::g_client_state->m_last_cmd_out
			&& shot_cmd_number <= (valve::g_client_state->m_last_cmd_out + valve::g_client_state->m_choked_cmds + 1);

		const auto speed = ((anim_state->m_walk_to_run_transition * 20.f) + 30.f) * valve::g_global_vars->m_interval_per_tick;

		float delta = 0.f;
		if (sdk::g_config_system->custom_desync) {
			switch (sdk::g_config_system->desync_mode) {
				case 1:
					{
						static bool jitter_switch = false;
						if (!valve::g_client_state->m_choked_cmds)
							jitter_switch = !jitter_switch;
						delta = (side == 1 ? sdk::g_config_system->desync_amount : -sdk::g_config_system->desync_amount) * (jitter_switch ? 1.f : -1.f);
					}
					break;
				case 2:
					delta = side == 1 ? sdk::g_config_system->desync_amount : -sdk::g_config_system->desync_amount;
					break;
				case 3:
					{
						const auto random_factor = g_context->addresses().m_random_int(0, 100);
						const auto random_amount = sdk::g_config_system->desync_amount * (random_factor / 100.f);
						delta = (side == 1 ? random_amount : -random_amount) * sdk::g_config_system->desync_inverter;
					}
					break;
				default:
					delta = side == 1 ? 60.f : -60.f;
					break;
			}
		} else {
			delta = side == 1 ? 60.f : -60.f;
		}

		bool should_fake_peek = sdk::g_config_system->fake_peek && key_handler::check_key(sdk::g_config_system->fake_peek_key, sdk::g_config_system->fake_peek_key_style);
		
		bool using_fake_peek_or_head = false;
		if (should_fake_peek) {
			valve::c_player* closest_enemy = nullptr;
			float closest_distance = std::numeric_limits<float>::max();
			
			for (auto i = 1; i <= valve::g_global_vars->m_max_clients; ++i) {
				const auto player = static_cast<valve::c_player*>(
					valve::g_entity_list->find_entity(i)
				);
				if (!player
					|| player->dormant()
					|| !player->alive()
					|| player->friendly()
					|| player == valve::g_local_player)
					continue;

				const auto dist = (valve::g_local_player->origin() - player->origin()).length();
				if (dist < closest_distance) {
					closest_distance = dist;
					closest_enemy = player;
				}
			}

			const auto& local_data = g_eng_pred->local_data().at(valve::g_client_state->m_last_cmd_out % 150);
			const auto current_lby_delta = math::angle_diff(local_data.m_anim_state.m_eye_yaw, local_data.m_anim_state.m_foot_yaw);
			
			const bool break_lby = sdk::g_config_system->fake_peek_type == 1;
			
			float base_delta = side == 1 ? 90.f : -90.f;
			
			if (closest_enemy && closest_distance > 0.f) {
				const auto speed_2d = anim_state->m_velocity.length_2d();
				const bool is_moving = speed_2d > 5.f;
				const bool is_crouching = valve::g_local_player->flags() & valve::e_ent_flags::ducking;
				
				float distance_factor = 1.f;
				if (closest_distance < 300.f) {
					distance_factor = 0.85f + (closest_distance / 300.f) * 0.15f;
				} else if (closest_distance > 800.f) {
					distance_factor = 1.15f;
				}

				float speed_factor = 1.f;
				if (is_moving) {
					const auto normalized_speed = std::min(speed_2d / 250.f, 1.f);
					speed_factor = 0.9f + normalized_speed * 0.2f;
				} else {
					speed_factor = 1.1f;
				}

				float crouch_factor = is_crouching ? 0.95f : 1.f;
				
				base_delta *= distance_factor * speed_factor * crouch_factor;
				
				if (std::abs(current_lby_delta) < 5.f) {
					if (!break_lby) {
						constexpr float LBY_LIMIT = 58.f;
						const auto max_safe_delta = LBY_LIMIT - std::abs(current_lby_delta) - 2.f;
						base_delta = std::copysign(std::min(std::abs(base_delta), max_safe_delta), base_delta);
					}
				} else {
					const auto smooth_factor = std::min(std::abs(current_lby_delta) / 60.f, 1.f);
					const auto target_delta = current_lby_delta * (1.f - smooth_factor * 0.3f) + base_delta * (smooth_factor * 0.3f);
					if (!break_lby) {
						constexpr float LBY_LIMIT = 58.f;
						const auto max_safe_delta = LBY_LIMIT - std::abs(current_lby_delta) - 2.f;
						base_delta = std::copysign(std::min(std::abs(target_delta), max_safe_delta), target_delta);
					} else {
						base_delta = target_delta;
					}
				}
			}
			
			if (!break_lby) {
				constexpr float LBY_LIMIT = 58.f;
				const auto max_safe_delta = LBY_LIMIT - std::abs(current_lby_delta) - 2.f;
				base_delta = std::copysign(std::min(std::abs(base_delta), max_safe_delta), base_delta);
				
				if (max_safe_delta < 10.f) {
					base_delta = std::copysign(std::max(5.f, max_safe_delta * 0.5f), base_delta);
				}
			}
			
			delta = base_delta;
			using_fake_peek_or_head = true;
		}

		if (!in_shot && !using_fake_peek_or_head) {
			const auto upper_limit = sdk::g_config_system->yaw_limit + speed;
			if (delta > upper_limit)
				delta = upper_limit;
			else {
				const auto lower_limit = (sdk::g_config_system->yaw_limit * -1.f) - speed;
				if (lower_limit > delta)
					delta = lower_limit;
			}
		}

		if (choked_cmds <= 0) {
			user_cmd.m_view_angles.y = std::remainder(yaw, 360.f);

			return;
		}

		if (!using_fake_peek_or_head) {
			const auto& local_data = g_eng_pred->local_data().at(valve::g_client_state->m_last_cmd_out % 150);
			if (std::abs(delta - 5.f) > std::abs(math::angle_diff(local_data.m_anim_state.m_eye_yaw, local_data.m_anim_state.m_foot_yaw)))
				delta = std::copysign(120.f, delta);
		}

		user_cmd.m_view_angles.y = std::remainder(yaw - delta, 360.f);
	}
}