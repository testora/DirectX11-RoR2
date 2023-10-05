#pragma once
#include "Client_Define.h"
#include "Selector.h"

BEGIN(Client)

class CBrotherNodeSelector_Phase1 final : public CSelector
{
private:
	explicit CBrotherNodeSelector_Phase1() DEFAULT;
	virtual ~CBrotherNodeSelector_Phase1() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>);

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	const BROTHER_PHASE*							m_pPhase	= nullptr;

public:
	static shared_ptr<CBrotherNodeSelector_Phase1>	Create(shared_ptr<class CBlackBoard>);
};

END

/*
Phase 1
페이즈 1

Mithrix starts the battle with basic attacks, running towards the player to get into melee range.
미스릭스는 기본 공격으로 전투를 시작하여, 플레이어에게 근접 범위로 접근하기 위해 달린다.

Primary: Hammer Smash/Needle Barrage
주 공격: 해머 스매시/니들 바라지

This skill has two modes depending on whether Mithrix is running or not.
이 스킬은 미스릭스가 달리고 있는지 여부에 따라 두 가지 모드가 있다.

If Mithrix is not running, he can use Hammer Smash, where he stops and winds up to smash the ground with his hammer.
미스릭스가 달리고 있지 않으면, 그는 해머로 땅을 내리치기 위해 멈추고 휘두를 수 있다.

A direct hit from the hammer's path deals 1200% with a proc coefficient of 1.0.
해머의 직접 타격은 1.0의 프록 계수로 1200%의 피해를 준다.

As the hammer hits the ground, it creates an explosion also dealing 1200% damage with a proc coefficient of 1.0 in a radius of 12m with linear falloff.
해머가 땅에 닿으면, 선형 감소를 가진 12m 반경에서 프록 계수 1.0으로 1200%의 피해를 주는 폭발을 일으킨다.

The combination of both sources of damage can kill the player, but if the first triggers one-shot protection, the blast will not damage the player as it occurs in less than 0.1s from when OSP has been triggered.
두 피해원의 조합은 플레이어를 죽일 수 있지만, 첫 번째가 원샷 보호를 발동시키면, OSP가 발동된 이후 0.1초 미만의 시간 동안 폭발은 플레이어에게 피해를 주지 않는다.

The attack lasts for 4 seconds, during which Mithrix is open to attacks.
이 공격은 4초 동안 지속되며, 미스릭스는 이 기간 동안 공격을 받을 수 있다.

However, attack speed affects the animation of the wind up and how quickly the smash will occur from the moment the skill is activated.
그러나 공격 속도는 스킬이 활성화된 순간부터 스매시가 얼마나 빨리 일어날지에 영향을 미친다.

This skill has a cooldown of 4 seconds.
이 스킬은 재사용 대기시간이 4초이다.

If Mithrix is sprinting instead, he can fire up to 12 homing needles moving at 120m/s.
만약 미스릭스가 달리고 있다면, 그는 120m/s로 움직이는 최대 12개의 유도 바늘을 발사할 수 있다.

Each needle explodes upon impact dealing 60% damage with a proc coefficient of 1.0 in a radius of 2m with no falloff.
각 바늘은 충돌시 2m의 반경 내에서 감소 없이 1.0의 프록 계수로 60%의 피해를 주며 폭발한다.

This skill has a cooldown of 6 seconds, which begins after the first needle is fired.
이 스킬의 재사용 대기시간은 6초이며, 첫 번째 바늘이 발사된 후에 시작된다.

Once the skill has come off cooldown, it will replenish all 12 stocks regardless of how many have been used.
스킬의 재사용 대기시간이 끝나면, 사용된 개수와 상관없이 모든 12개의 바늘이 보충된다.

If Mithrix toggles sprinting while the skill is on cooldown so that it switches to Hammer Smash and back, all charges will be replenished immediately.
미스릭스가 스킬이 재사용 대기시간 동안 달리기를 전환하여 해머 스매시로 바꾸고 다시 되돌리면, 모든 충전이 즉시 보충된다.

Secondary: Hammer Swing
보조 공격: 해머 스윙

Mithrix swipes with his hammer, dealing 200% damage with a proc coefficient of 1.0 while pushing the target away.
미스릭스는 해머로 휘두르며, 1.0의 프록 계수로 200%의 피해를 주고 대상을 밀어낸다.

This attack moves him forwards a moderate amount.
이 공격은 그를 적절한 거리만큼 앞으로 이동시킨다.

This skill has a cooldown of 5 seconds and the attack animation lasts for 4 seconds which scales with attack speed.
이 스킬은 재사용 대기시간이 5초이고, 공격 애니메이션은 4초 동안 지속되며 이는 공격 속도에 따라 조절된다.

Utility: Dash
유틸리티: 대시

Mithrix dashes a short distance forward, backward, left, or right for 0.3 seconds.
미스릭스는 0.3초 동안 앞, 뒤, 왼쪽, 또는 오른쪽으로 짧은 거리를 대시한다.

This also removes all his timed debuffs, but not any damage-over-time effects.
이로써 그의 모든 시간제 디버프는 제거되지만, 지속 피해 효과는 제거되지 않는다.

This skill has a cooldown of 3 seconds and holds 2 charges.
이 스킬은 재사용 대기시간이 3초이며, 2회의 충전을 보유한다.

Special: Shock Wave
특수: 쇼크 웨이브

Mithrix bends down to jump high into the air, which he does after 1 second (scales with attack speed).
미스릭스는 1초 후에(공격 속도에 따라 조절) 높이 뛰기 위해 몸을 숙인다.

After 3 seconds he slams down in the center of the arena, creating a ring-shaped shockwave consisting of 12 waves that travel outwards at 60m/s for 3 seconds.
3초 후에 그는 아레나 중앙에 착지하여, 3초 동안 60m/s로 바깥쪽으로 이동하는 12개의 파동으로 구성된 반지 모양의 쇼크웨이브를 생성한다.

The shock wave effectively ends when it reaches the thin line of rocks between the platform where he is first seen.
쇼크웨이브는 그가 처음 볼 수 있는 플랫폼 사이의 얇은 바위 줄에 도달하면 효과적으로 끝난다.

Each wave deals 400% with a proc coefficient of 1.0 and inflicts Status Cripple(Armor by 20, Speed by 50%).
각 파동은 1.0의 프록 계수로 400%의 피해를 주고 상태 저하(아머 20, 이동속도 50%)를 유발한다.
 
This skill has a cooldown of 30 seconds.
이 스킬의 재사용 대기시간은 30초이다.

Behaviors
행동

Use special (Shock Wave): Mithrix must have less than 75% health.
특수 사용 (쇼크 웨이브): 미스릭스의 체력이 75% 미만이어야 한다.

Use utility and strafe: Mithrix must be within 40m from its target, This behavior cannot be selected twice in a row.
유틸리티와 사이드 스텝 사용: 미스릭스는 대상과 40m 이내에 있어야 하며, 이 행동은 연속으로 두 번 선택될 수 없다.

If this behavior is selected as Mithrix is approaching the player, it will very likely be followed up by either his primary or secondary skill.
이 행동이 미스릭스가 플레이어에게 접근하는 도중에 선택되면, 그의 주 공격 또는 보조 공격으로 이어질 가능성이 매우 높다.

This can look like strafing out of the way of a possible incoming attack and then attacking from the flank.
이것은 가능한 들어오는 공격을 피하기 위해 옆으로 움직이고, 그 다음에 측면에서 공격하는 것처럼 보일 수 있다.

Use utility: Mithrix must be within 40-50m from its target. This behavior cannot be selected twice in a row.
유틸리티 사용: 미스릭스는 대상과 40-50m 이내에 있어야 하며, 이 행동은 연속으로 두 번 선택될 수 없다.

Use secondary: Mithrix must be within 40m from its target.
보조 공격 사용: 미스릭스는 대상과 40m 이내에 있어야 한다.

Use primary (Hammer Smash): Mithrix must be within 35m from its target.
주 공격 사용 (해머 스매시): 미스릭스는 대상과 35m 이내에 있어야 한다.

Sprint and use primary (Needle Barrage): Mithrix must be more than 70m away from its target, have line of sight, and have less than 80% health.
달리고 주 공격 사용 (니들 바라지): 미스릭스는 대상과 70m 이상 떨어져 있어야 하며, 시야가 있어야 하고, 체력이 80% 미만이어야 한다.

Mithrix will ignore the node graph and run directly towards the target. This behavior is active for 2 seconds.
미스릭스는 노드 그래프를 무시하고 대상을 향해 직접 달린다. 이 행동은 2초 동안 활성화된다.

Chase off nodegraph: Mithix must be within 50m from its target and have line of sight.
노드 그래프를 벗어나 추격: 미스릭스는 대상과 50m 이내에 있어야 하며, 시야가 있어야 한다.

This behavior is active for 2 seconds and will most likely be selected when Mithrix is near the target after both the primary and secondary are on cooldown.
이 행동은 2초 동안 활성화되며, 주 공격과 보조 공격 모두 쿨다운 중일 때 미스릭스가 대상에 가까우면 가장 가능성이 높게 선택될 것이다.

Sprint off nodegraph: Mithrix must be more than 50m away from its target. This behavior is active for 1 second.
노드 그래프를 벗어나 달리기: 미스릭스는 대상과 50m 이상 떨어져 있어야 한다. 이 행동은 1초 동안 활성화된다.

Chase: no requirements.
추격: 요구사항 없음.
*/
