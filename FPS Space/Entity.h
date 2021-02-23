#include <SFML/Graphics.hpp>
using namespace sf;

class Entity : public Drawable, public Transformable {
public:
	Entity(IntRect rect) {
		sprite.setTexture(texture);
		sprite.setTextureRect(rect);

		// ����� ������ �� ��������� (������������� �������)
		outline.setPrimitiveType(LineStrip);
		outline.resize(4);
		this->outline[0].position = Vector2f(0, 0);
		this->outline[1].position = Vector2f(0, rect.height);
		this->outline[2].position = Vector2f(rect.width, rect.height);
		this->outline[3].position = Vector2f(rect.width, 0);

		// ���������� �������� ������ � ������ �� �������
		width = rect.width;
		height = rect.height;
	}
	
	// �������������� getPosition(), ����� ������������ �� ��������, � �� ����� ����� �������
	const Vector2f &getPosition() const {
		return sprite.getPosition();
	}
	
	// �������������� setPosition(), ����� ��������� ������� ������� ������������ ������ � ����� �������� (��������)
	void setPosition(float x, float y) {
		float offsetX = x - getPosition().x;
		float offsetY = y - getPosition().y;
		sprite.setPosition(x, y);
		for (int i = 0; i < outline.getVertexCount(); i++) {
			outline[i].position.x += offsetX;
			outline[i].position.y += offsetY;
		}
	}
	void setPosition(Vector2f pos) {
		float offsetX = pos.x - getPosition().x;
		float offsetY = pos.y - getPosition().y;
		sprite.setPosition(pos);
		for (int i = 0; i < outline.getVertexCount(); i++) {
			outline[i].position.x += offsetX;
			outline[i].position.y += offsetY;
		}
	}

	// �������������� move()
	void move(float offsetX, float offsetY) {
		setPosition(getPosition().x + offsetX, getPosition().y + offsetY);
	}
	void move(const Vector2f &offset) {
		setPosition(getPosition() + offset);
	}

	// �������� ������� ������ � ������ �������(�������)
	const float getWidth() const {
		return width;
	}
	const float getHeight() const {
		return height;
	}

	// �������� ������� ������� �������
	const VertexArray getOutline()  const {
		return outline;
	}

	//������������� ��� ��������� �������� �������, ������������ ������ ����� setPosition(), ����� ������� ����� ����� �� �������, ��� � ������(������)
	void setOutline(std::vector<Vector2f> outline) {
		this->outline.clear();
		this->outline.resize(outline.size());
		for (unsigned int i = 0; i < outline.size(); i++)
			this->outline[i].position = outline[i];
	}
	
	// ����� �������� ����������� �������� �������� ������� � ��������, ���������� � �������� ���������
	bool intersects(const VertexArray outline) {
		// ���������� �� ����� ������� ����� ������� ������� � �� ������ ���� ���� �������(������) y(x) = kx + c, ������� �������� ����� ��� ��� �����
		for (unsigned int i = 1; i < this->outline.getVertexCount(); i++)
		{
			// ����������� ������� ������
			long double k1;
			// ���������� ����������, ����������� true, ���� ������������ ������ ����� �� �������������� (�� y = c) � false � ��������� ������
			bool y1 = true;

			// ���� ��� ������ �� y=c, �� ������� k
			if (this->outline[i].position.x - this->outline[i - 1].position.x != 0)
				k1 = (this->outline[i].position.y - this->outline[i - 1].position.y) / (this->outline[i].position.x - this->outline[i - 1].position.x);
			else
			{
				k1 = 1;
				y1 = false;
			}
			// ��������� ��������� ������ ������
			long double c1;

			// ���� �-� �� ���� y=c
			if (y1)
				// ������� �� ������� c = y - kx (������� �� y = kx + c)
				c1 = this->outline[i].position.y - this->outline[i].position.x * k1;
			else
				// � ��������� ������ ������ c = y (�� y = c) (� ������� ��-�� ���������� ������������ ��������� � sfml)
				c1 = -this->outline[i].position.x;

			// ������ ��� ����������� ������ �� ������� ������� ������� - �������� ����� ���� ��������� (*)
			for (unsigned int j = 1; j < outline.getVertexCount(); j++)
			{
				// (*)
				long double k2;
				bool y2 = true;
				if (outline[j].position.x - outline[j - 1].position.x != 0)
					k2 = (outline[j].position.y - outline[j - 1].position.y) / (outline[j].position.x - outline[j - 1].position.x);
				else
				{
					k2 = 1;
					y2 = false;
				}

				long double c2;
				if (y2)
					c2 = outline[j].position.y - outline[j].position.x * k2;
				else
					c2 = -outline[j].position.x;
				// (*)


				// ���� k1*x + c1 == k2*x + c2 � ��� ������ �� ��� y=c
				if (c2 - c1 == 0 && k2 - k1 == 0 && y1 == y2)
					// ������ ������������
					return true;
				// ���� ��� ����������� => ��������� � ��������� ���� ������
				else if (k2 - k1 == 0 && y1 == y2)
					continue;
				else
				{
					long double x;
					long double y;

					// � ����������� �� ���� ������ ������ ���� ����� ����������� ������ [� ���������� (x, y)]
					if (!y1 && y2)
					{
						x = -c1;
						y = k2 * x + c2;
					}
					else if (y1 && !y2)
					{
						x = -c2;
						y = k1 * x + c1;
					}
					else
					{
						x = -(c2 - c1) / (k2 - k1);
						y = k1 * x + c1;
					}
					// ���� ��������� ����������  ������ � ���������� ������ �������� ��������
					if (fminf(this->outline[i].position.y, this->outline[i - 1].position.y) <= y && y <= fmaxf(this->outline[i].position.y, this->outline[i - 1].position.y) &&
						fminf(this->outline[i].position.x, this->outline[i - 1].position.x) <= x && x <= fmaxf(this->outline[i].position.x, this->outline[i - 1].position.x) &&
						fminf(outline[j].position.y, outline[j - 1].position.y) <= y && y <= fmaxf(outline[j].position.y, outline[j - 1].position.y) &&
						fminf(outline[j].position.x, outline[j - 1].position.x) <= x && x <= fmaxf(outline[j].position.x, outline[j - 1].position.x))
					{
						// ������� ������������
						return true;
					}
				}
			}
		}
		// �� ���� ��������� ������� ������� �� ������������
		return false;
	}

	// ����� "��������" ������� 
	void die() {
		life = false;
	}
	
	// ������� �������������� ������ ���������� life
	const bool isLife() const {
		return life;
	}
	
protected:
	// ���������� ����� ��� ���������� ������, ���������� � �������� update �������� �������
	void explosion(float time) {
		// ���� �� �� ������ ��� 74 ����� �������� ������
		if (currentFrameExplosion <= 74)
			// �������� �������� �� ��������� ����
			sprite.setTextureRect(IntRect(130 + (int(currentFrameExplosion) % 9) * 100, (int(currentFrameExplosion) / 9) * 100, 100, 100));
		else
			// ����� ���������� ������ 
			die();
		// �������� ������� ����� �������� � ��������
		currentFrameExplosion += time * speedExplosion;
	}

private:
	// ����������� ����� draw, ����� �������� ���� �������, � �� �� �������
	virtual void draw(RenderTarget& target, RenderStates states) const
	{
		target.draw(sprite);
		// ����� ��������������� ������ ���� � ����������������� ������ ����, ����� ������� ������� �������� ��������
		//target.draw(outline, states);
	}

	bool life = true;						// ���������� ���������� ������������ ��� ������ ��� ���
	float width;							// ������ � ������ �������� �������
	float height;
	float speedExplosion = 0.1;				// �������� �������� ������
	float currentFrameExplosion = 0;		// ������� ���� ������
	Sprite sprite;							// ������ ������� �������
	VertexArray outline;					// ������ ����� ��� ������� �������� (����������� � ����� ������� �����) (�� �� ������ �������)
};
