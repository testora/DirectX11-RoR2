#pragma once
#include "Client_Define.h"
#include "Selector.h"

BEGIN(Engine)
END

BEGIN(Client)

class CBrotherNodeSelector_Phase3 final : public CSelector
{
private:
	explicit CBrotherNodeSelector_Phase3() DEFAULT;
	virtual ~CBrotherNodeSelector_Phase3() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	BROTHER_PHASE*									m_pPhase	= nullptr;

public:
	static shared_ptr<CBrotherNodeSelector_Phase3>	Create(shared_ptr<class CBlackBoard>);
};

END

/*
Phase 3
Mithrix returns with his hammer glowing blue and some of his attacks are now upgraded. Lunar enemies will also continue spawning occasionally.

페이즈 3
Mithrix가 파란색으로 빛나는 망치와 함께 돌아오며 일부 공격이 업그레이드됩니다. 루나(달) 적들도 가끔씩 계속해서 생성됩니다.

Primary: Lunar Hammer Smash/Needle Barrage
Lunar Hammer Smash와 Needle Barrage가 주 공격입니다.

Mithrix's Hammer Smash is now energized and upon explosion it will create two additional effects.
Mithrix의 Hammer Smash는 이제 에너지가 꽉 차고 폭발 시 두 가지 추가 효과를 생성합니다.
3 small shock waves are sent forward, similar to those from his Shock Wave skill.
개의 작은 충격파가 전방으로 발사되며, 이는 그의 Shock Wave 스킬과 유사합니다.
These deal 200% with a proc coefficient of 1.0 and also inflict Cripple.
이것들은 1.0의 proc 계수와 함께 200%의 데미지를 주고 또한 Cripple을 가합니다.

A lingering pillar of blue flame is also created at the hammer's point of impact, which deals 75% damage with a proc coefficient of 0.2 and ticks 6 times per second for 45 seconds.
또한 망치가 땅에 닿은 지점에서 파란 불의 기둥이 생겨 0.2의 proc 계수와 함께 75%의 데미지를 입히고 45초 동안 초당 6번씩 데미지를 줍니다.

These pillars are 40m tall and can go through terrain.
이 기둥들은 40m 높이가 있고 지형을 관통할 수 있습니다.
This means that if the player is on top of a ramp on the outer areas of the arena and Mithrix smashes the ground underneath, the pillar can go through the ramp's ceiling and damage the player.
이는 플레이어가 아레나 외곽의 경사로 위에 있고 Mithrix가 아래쪽을 내리쳤을 경우, 이 기둥이 경사로의 천장을 관통하여 플레이어에게 데미지를 줄 수 있다는 것을 의미합니다.
These pillars can also be blocked by Defensive Microbots.
이 기둥들은 또한 Defensive Microbots에 의해 차단될 수 있습니다.

Special: Shock Wave/Big Spinny
특별 공격: Shock Wave/Big Spinny

After Mithrix performs his Shock Wave leap, his special skill is replaced by a new one which becomes available after a cooldown of 8 seconds.
Mithrix가 Shock Wave 점프를 수행한 후에는 그의 특별 스킬이 8초의 쿨다운 후에 사용 가능한 새로운 스킬로 바뀝니다.

Once this new skill has been activated, it will revert back to Shock Wave.
이 새로운 스킬이 활성화되면, 다시 Shock Wave로 되돌아갑니다.

With this new skill Mithrix creates 9 pillars that rotate either clockwise or anticlockwise.
이 새로운 스킬로 Mithrix는 시계 방향 또는 반시계 방향으로 회전하는 9개의 기둥을 만듭니다.
After 3 seconds they deal damage on the spot for 900% damage with a proc coefficient of 1.0 and also inflict Cripple.
3초 후에 이들은 1.0의 proc 계수와 함께 현장에서 900%의 데미지를 주고 또한 Cripple을 가합니다.

Mithrix will execute this attack 5 times in a row, with each time the rotation being random.
Mithrix는 이 공격을 연속으로 5번 실행할 것이며, 매번 회전 방향은 랜덤합니다.

Behaviors
행동

Mithrix behaves the same away as in Phase 1, but he has one new behavior that supersedes every other in terms of priority for his new special attack.
Mithrix는 페이즈 1과 마찬가지로 행동하지만, 그의 새로운 특별 공격에 대한 우선순위에서 모든 다른 것보다 우선하는 새로운 행동이 하나 있습니다.

Use special (Big Spinny): Mithrix must have less than 80% health.
특별 공격 사용 (Big Spinny): Mithrix의 체력이 80% 미만이어야 합니다.

In theory this will always be selected when the skill comes off cooldown after Shock Wave, since the latter requires less than 75% health to be used.
이론적으로 이것은 Shock Wave 후에 스킬 쿨다운이 끝나면 항상 선택될 것입니다, 후자는 사용하기 위해 75% 미만의 체력이 필요하기 때문입니다.

However, if Mithrix has healing items from the Artifact of Evolution, it is possible to use Shock Wave after having less than 75% health, but then recover to more than 80% before Big Spinny comes off cooldown.
그러나, Mithrix가 Evolution Artifact of Evolution에서 회복 아이템을 가지고 있다면, 75% 미만의 체력을 가진 상태에서 Shock Wave를 사용한 후에 Big Spinny의 쿨다운이 끝나기 전에 80% 이상으로 회복할 수 있습니다.
*/
