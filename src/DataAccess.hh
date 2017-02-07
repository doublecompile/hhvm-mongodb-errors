<?hh

namespace Crashy;

use MongoDB\Driver\BulkWrite;
use MongoDB\Driver\Cursor;
use MongoDB\Driver\Manager;
use MongoDB\Driver\Query;
use MongoDB\Driver\ReadPreference;
use MongoDB\Driver\WriteConcern;
use MongoDB\Driver\WriteResult;

class DataAccess
{
    private array<string,?string> $typeMap = ['root' => null, 'document' => null];

    private ?ReadPreference $readPreference;

    private ?WriteConcern $writeConcern;

    public function __construct(
        private Manager $manager,
        private string $collection,
        ?\ConstMap<string,mixed> $options = null,
    ) {
        if ($options !== null) {
            if ($options->containsKey('typeMapRoot')) {
                $r = $options['typeMapRoot'];
                $this->typeMap['root'] = $r === null ? null : (string)$r;
            }
            if ($options->containsKey('typeMapDocument')) {
                $d = $options['typeMapDocument'];
                $this->typeMap['document'] = $d === null ? null : (string)$d;
            }
            $rp = $options['readPreference'] ?? null;
            if ($rp instanceof ReadPreference) {
                $this->readPreference = $rp;
            }
            $wc = $options['writeConcern'] ?? null;
            if ($wc instanceof WriteConcern) {
                $this->writeConcern = $wc;
            }
        }
    }

    public function query(array<string,mixed> $criteria, array<string,mixed> $options = []): Cursor
    {
        $results = $this->doExecute(function (Manager $m, string $c) use ($criteria, $options) {
            $q = new Query($criteria, $options);
            $res = $m->executeQuery($c, $q, $this->readPreference);
            $res->setTypeMap($this->typeMap);
            return $res;
        });
        return $results;
    }

    public function persist(\MongoDB\BSON\Persistable $record): WriteResult
    {
        $wr = $this->doExecute(function (Manager $m, string $c) use ($record) {
            $bulk = new BulkWrite();
            $bulk->insert($record);
            return $m->executeBulkWrite($c, $bulk, $this->writeConcern);
        });
        return $wr;
    }

    public function update(array<string,mixed> $criteria, array<string,mixed> $ops): WriteResult
    {
        $wr = $this->doExecute(function (Manager $m, string $c) use ($criteria, $ops) {
            $bulk = new BulkWrite();
            $bulk->update($criteria, $ops);
            return $m->executeBulkWrite($c, $bulk, $this->writeConcern);
        });
        return $wr;
    }

    protected function doExecute<T>((function(Manager,string): T) $cb): T
    {
        try {
            return $cb($this->manager, $this->collection);
        } catch (\Exception $e) {
            // normally, we'd do something here.
            throw new \RuntimeException("Exception. All is lost.", 0, $e);
        }
    }
}
