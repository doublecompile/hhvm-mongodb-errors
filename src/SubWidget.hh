<?hh

namespace Crashy;

use MongoDB\BSON\Persistable;

class SubWidget implements Persistable, \JsonSerializable
{
    public function __construct(
        private string $name,
        private float $value,
        private bool $enabled
    ) {
    }

    public static function from(mixed $value): ?SubWidget
    {
        if ($value instanceof SubWudget) {
            return $value;
        } elseif ($value instanceof KeyedTraversable) {
            $values = new Map($value);
            return new SubWidget((string) $values->get('name'),
                (float) $values->get('value'), (bool) $values->get('enabled'));
        } elseif (is_object($value)) {
            return self::from(get_object_vars($value));
        }
        return null;
    }

    public function getName(): string
    {
        return $this->name;
    }

    public function getValue(): float
    {
        return $this->value;
    }

    public function isEnabled(): bool
    {
        return $this->enabled;
    }

    public function jsonSerialize(): mixed
    {
        return $this->bsonSerialize();
    }

    public function bsonSerialize(): mixed
    {
        return ['name' => $this->name, 'value' => $this->value, 'enabled' => $this->enabled];
    }

    public function bsonUnserialize(array<arraykey,mixed> $data): void
    {
        foreach ($data as $k => $v) {
            if ($k === 'name') {
                $this->name = (string) $v;
            } elseif ($k === 'value') {
                $this->value = (float) $v;
            } elseif ($k === 'enabled') {
                $this->enabled = (bool) $v;
            }
        }
    }
}
