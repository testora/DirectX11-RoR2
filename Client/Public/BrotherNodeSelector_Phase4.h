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
������ 4

Mithrix appears at the center of the arena and looks weakened with his armor broken and his hammer gone.
�̽������� �Ʒ��� �߾ӿ� ��Ÿ����, ���� ������ �μ����� ��ġ�� ������ ������ ����� ���Դϴ�.

He starts by stealing all players' and their allies' items, during which time he is invulnerable.
�״� ��� �÷��̾�� �׵��� ������ �������� ��ġ�� �����ϸ�, �� �Ⱓ ���ȿ��� �����Դϴ�.

Damaging him will release the items.
�׸� �����ϸ� �����۵��� ��ȯ�˴ϴ�.

See the Notes section for more details of how this works.
�� �۵� ��Ŀ� ���� �ڼ��� ������ ��Ʈ ������ �����Ͻʽÿ�.

The stealing of the player's items can be skipped if Mithrix takes damage in the first frame of the phase.
�������� ù �����ӿ��� �̽������� ���ظ� ������ �÷��̾��� ������ ��ġ�⸦ �ǳʶ� �� �ֽ��ϴ�.

This is due to his invulnerability being applied after a short window, and not being present on spawn.
�̰��� ���� ������ ª�� �ð� �Ŀ� ����ǰ�, ������ ���� �������� �ʱ� �����Դϴ�.

This is most easily done with continuous damage, most notably a Kjaro's Band triggered in the center of the arena.
�̰��� �Ʒ��� �߾ӿ��� �ߵ��� Kjaro's Band�� ���� �������� ���ط� ���� ���� ����� �� �ֽ��ϴ�.

(which may occur when Mithrix's phase 3 is finished after his jump slam)
(�̰��� �̽������� ������ 3�� ���� ���� ���� �Ŀ� ���� �� �߻��� �� �ֽ��ϴ�)

This will leave Mithrix with no items, and the player with all of their items, resulting in a quick completion of this phase.
�̷� ���� �̽������� �������� ���� �ǰ�, �÷��̾�� ��� �������� ������ �Ǿ� �� ����� ������ �Ϸ��� �� �ֽ��ϴ�.

Mithrix moves much slower than in the previous phases, slowly stomping around the arena and using exclusively ranged attacks.
�̽������� ���� ������� �ξ� ������ �����̸�, �Ʒ����� õõ�� ����� �������� ���Ÿ� ������ ����մϴ�.

While his base damage is also weaker, the use of the stolen items can cause devastation.
�⺻ ���ص� ��������, ��ģ �������� ����ϸ� ��ȥ���� ����ų �� �ֽ��ϴ�.

His HP is governed by a different formula and since he no longer has unique scaling, his max HP will be significantly lower.
���� HP�� �ٸ� ���Ŀ� ���� �����Ǹ�, �� �̻� ��Ư�� �����ϸ��� ���� ������ ���� �ִ� HP�� �ξ� �������ϴ�.

Any single instance of damage that deals more than 30% of his health will also stagger him.
���� �ǰ��� 30% �̻��� ���ϴ� ���� ���� �ν��Ͻ��� �׸� ��Ʋ�� ����ϴ�.

If the player has any Halcyon Seeds, Aurelionite will spawn and help the player during this phase.
�÷��̾ Halcyon Seeds�� ������ �ִٸ�, �� ������ ���� Aurelionite�� �����Ǿ� �÷��̾ ���� ���Դϴ�.

There is a bug in which Mithrix will give the player Tonic Afflictions without either of them previously having any.
�̽������� �÷��̾�� Tonic Afflictions�� �ִ� ���װ� ������, �̴� �׵� �� ��� �ʵ� ������ ������ ���� �ʾ��� �� �߻��մϴ�.

Primary: Needle Burst
�⺻ ����: �ϵ� ����Ʈ

Similar to his needle attack from the previous phases, but there is less spread.
���� �������� �ϵ� ���ݰ� ����������, ������ ���մϴ�.

Secondary: Lunar Orbs
���� ����: �糪 ����

Mithrix kneels and pounds the ground, creating a blast that deals 600% damage with a proc coefficient of 1.0 in a radius of 12m with linear falloff.
�̽������� ������ �ݰ� ���� ����ģ��, �̷ν� 12m �ݰ� ������ ���� ���Ҹ� ���� 600%�� ���ؿ� proc coefficient 1.0�� ������ �������ϴ�.

This also creates a ring of 8 orbs that spiral outwards, converge back into the center and then spiral out again.
�̷ν� 8���� ���갡 ���ͼ� �ٱ����� �����ٰ� �ٽ� �߾����� ���̰�, �ٽ� �ٱ����� �����ϴ�.

Upon impact they deal 400% damage with a proc coefficient of 1.0 in a radius of 6m with linear falloff.
�浹 �ÿ��� 6m �ݰ濡�� ���� ���Ҹ� ���� 400%�� ���ؿ� proc coefficient 1.0�� ���ظ� �ݴϴ�.

Their lifetime is 9s (plus a random 0.0-0.3 second offset), after which they will explode on the spot, so players should avoid being near them at that point.
�׵��� ������ 9��(������ 0.0-0.3�� ������ �߰�)�̸�, ���Ŀ��� �� �ڸ����� �����ϹǷ�, �÷��̾�� �� ������ ������ ���� �ʾƾ� �մϴ�.

Mithrix also deals 8% of his current health as non-lethal damage to himself.
�̽������� ���� ���� ���� �ǰ��� 8%�� �ڽſ��� ġ�������� ���� ���ط� �ݴϴ�.

This skill has an animation duration of 6 seconds which scales with attack speed.
�� ��ų�� ���� �ӵ��� ���� �����Ǵ� �ִϸ��̼� ���� �ð��� 6���Դϴ�.

This skill has a cooldown of 8 seconds, which does not start until the attack has finished.
�� ��ų�� ��ٿ��� 8���̸�, ������ ���� ������ ���۵��� �ʽ��ϴ�.

Behaviors
�ൿ ����

Use secondary: Mithrix must be within 180m from its target and have less than 90% health.
���� ���� ���: �̽������� ���� 180m �̳��� �־�� �ϸ� �ǰ��� 90% �̸��̾�� �մϴ�.

Use primary: no requirements as long as the skill is off cooldown.
�⺻ ���� ���: ��ų�� ��ٿ�� ��� �ִٸ� �䱸���� ����.

Chase: no requirements.
�߰�: �䱸���� ����.
*/
