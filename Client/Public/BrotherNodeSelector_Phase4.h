#pragma once
#include "Client_Define.h"
#include "Selector.h"

BEGIN(Engine)
END

BEGIN(Client)

class CBrotherNodeSelector_Phase4 final : public CSelector
{
private:
	explicit CBrotherNodeSelector_Phase4() DEFAULT;
	virtual ~CBrotherNodeSelector_Phase4() DEFAULT;

public:
	virtual HRESULT									Initialize(shared_ptr<class CBlackBoard>) override;

public:
	virtual void									Activate() override;
	virtual STATUS									Invoke(_float fTimeDelta) override;
	virtual void									Terminate() override;

private:
	BROTHER_PHASE*									m_pPhase	= nullptr;

public:
	static shared_ptr<CBrotherNodeSelector_Phase4>	Create(shared_ptr<class CBlackBoard>);
};

END

/*
Phase 4
페이즈 4

Mithrix appears at the center of the arena and looks weakened with his armor broken and his hammer gone.
미스릭스는 아레나 중앙에 나타나며, 그의 갑옷은 부서지고 망치는 없어져 약해진 모습을 보입니다.

He starts by stealing all players' and their allies' items, during which time he is invulnerable.
그는 모든 플레이어와 그들의 동맹의 아이템을 훔치기 시작하며, 이 기간 동안에는 무적입니다.

Damaging him will release the items.
그를 공격하면 아이템들이 반환됩니다.

See the Notes section for more details of how this works.
이 작동 방식에 대한 자세한 내용은 노트 섹션을 참고하십시오.

The stealing of the player's items can be skipped if Mithrix takes damage in the first frame of the phase.
페이즈의 첫 프레임에서 미스릭스가 피해를 입으면 플레이어의 아이템 훔치기를 건너뛸 수 있습니다.

This is due to his invulnerability being applied after a short window, and not being present on spawn.
이것은 그의 무적이 짧은 시간 후에 적용되고, 스폰할 때는 존재하지 않기 때문입니다.

This is most easily done with continuous damage, most notably a Kjaro's Band triggered in the center of the arena.
이것은 아레나 중앙에서 발동된 Kjaro's Band와 같은 지속적인 피해로 가장 쉽게 수행될 수 있습니다.

(which may occur when Mithrix's phase 3 is finished after his jump slam)
(이것은 미스릭스의 페이즈 3가 그의 점프 슬램 후에 끝날 때 발생할 수 있습니다)

This will leave Mithrix with no items, and the player with all of their items, resulting in a quick completion of this phase.
이로 인해 미스릭스는 아이템이 없게 되고, 플레이어는 모든 아이템을 가지게 되어 이 페이즈를 빠르게 완료할 수 있습니다.

Mithrix moves much slower than in the previous phases, slowly stomping around the arena and using exclusively ranged attacks.
미스릭스는 이전 페이즈보다 훨씬 느리게 움직이며, 아레나를 천천히 짓밟고 전적으로 원거리 공격을 사용합니다.

While his base damage is also weaker, the use of the stolen items can cause devastation.
기본 피해도 약하지만, 훔친 아이템을 사용하면 대혼란을 일으킬 수 있습니다.

His HP is governed by a different formula and since he no longer has unique scaling, his max HP will be significantly lower.
그의 HP는 다른 공식에 의해 조절되며, 더 이상 독특한 스케일링이 없기 때문에 그의 최대 HP는 훨씬 낮아집니다.

Any single instance of damage that deals more than 30% of his health will also stagger him.
그의 건강의 30% 이상을 가하는 단일 피해 인스턴스도 그를 비틀게 만듭니다.

If the player has any Halcyon Seeds, Aurelionite will spawn and help the player during this phase.
플레이어가 Halcyon Seeds를 가지고 있다면, 이 페이즈 동안 Aurelionite가 생성되어 플레이어를 도울 것입니다.

There is a bug in which Mithrix will give the player Tonic Afflictions without either of them previously having any.
미스릭스가 플레이어에게 Tonic Afflictions를 주는 버그가 있으며, 이는 그들 중 어느 쪽도 이전에 가지고 있지 않았을 때 발생합니다.

Primary: Needle Burst
기본 공격: 니들 버스트

Similar to his needle attack from the previous phases, but there is less spread.
이전 페이즈의 니들 공격과 유사하지만, 퍼짐이 덜합니다.

Secondary: Lunar Orbs
보조 공격: 루나 오브

Mithrix kneels and pounds the ground, creating a blast that deals 600% damage with a proc coefficient of 1.0 in a radius of 12m with linear falloff.
미스릭스는 무릎을 꿇고 땅을 내리친다, 이로써 12m 반경 내에서 선형 감소를 가진 600%의 피해와 proc coefficient 1.0의 폭발을 만들어냅니다.

This also creates a ring of 8 orbs that spiral outwards, converge back into the center and then spiral out again.
이로써 8개의 오브가 나와서 바깥으로 나갔다가 다시 중앙으로 모이고, 다시 바깥으로 나갑니다.

Upon impact they deal 400% damage with a proc coefficient of 1.0 in a radius of 6m with linear falloff.
충돌 시에는 6m 반경에서 선형 감소를 가진 400%의 피해와 proc coefficient 1.0의 피해를 줍니다.

Their lifetime is 9s (plus a random 0.0-0.3 second offset), after which they will explode on the spot, so players should avoid being near them at that point.
그들의 수명은 9초(무작위 0.0-0.3초 오프셋 추가)이며, 이후에는 그 자리에서 폭발하므로, 플레이어는 그 시점에 가까이 가지 않아야 합니다.

Mithrix also deals 8% of his current health as non-lethal damage to himself.
미스릭스는 또한 그의 현재 건강의 8%를 자신에게 치명적이지 않은 피해로 줍니다.

This skill has an animation duration of 6 seconds which scales with attack speed.
이 스킬은 공격 속도에 따라 조정되는 애니메이션 지속 시간이 6초입니다.

This skill has a cooldown of 8 seconds, which does not start until the attack has finished.
이 스킬은 쿨다운이 8초이며, 공격이 끝날 때까지 시작되지 않습니다.

Behaviors
행동 패턴

Use secondary: Mithrix must be within 180m from its target and have less than 90% health.
보조 공격 사용: 미스릭스는 대상과 180m 이내에 있어야 하며 건강이 90% 미만이어야 합니다.

Use primary: no requirements as long as the skill is off cooldown.
기본 공격 사용: 스킬이 쿨다운에서 벗어나 있다면 요구사항 없음.

Chase: no requirements.
추격: 요구사항 없음.
*/
