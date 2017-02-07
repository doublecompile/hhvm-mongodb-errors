<?hh

chdir(dirname(__DIR__));
require 'vendor/autoload.php';

$myUrl = (string) $_SERVER['REQUEST_URI'];

$hostname = $_ENV['MONGO_PORT_27017_TCP_ADDR'] ?? 'localhost';

$m = new MongoDB\Driver\Manager("mongodb://$hostname:27017", ['journal' => true]);
$rp = new MongoDB\Driver\ReadPreference(MongoDB\Driver\ReadPreference::RP_PRIMARY);
$wc = new MongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY, 1000);
$accessDao = new \Crashy\DataAccess($m, 'hhvm.accesses', Map{'typeMapRoot' => \Crashy\Access::class, 'readPreference' => $rp, 'writeConcern' => $wc});
$userDao = new \Crashy\DataAccess($m, 'hhvm.users', Map{'typeMapRoot' => \Crashy\User::class, 'readPreference' => $rp, 'writeConcern' => $wc});

$access = new \Crashy\Access($myUrl, [random_int(0, 25), random_int(26, 50), random_int(51, 75)]);
$accessDao->persist($access);

$users = $userDao->query(['email' => 'nobody@example.com'])->toArray();
if (count($users) == 0) {
    $user = new \Crashy\User([
        'email' => 'nobody@example.com',
        'displayName' => 'Nobody Special',
        'password' => 'correct horse battery staple',
        'address' => ['country' => 'US', 'region' => 'MD', 'locality' => 'Columbia'],
        'lastUrl' => $myUrl,
    ]);
    $userDao->persist($user);
} else {
    $user = current($users);
    $userDao->update(['_id' => $user->getId()], ['$set' => ['lastUrl' => $myUrl]]);
}

$results = new Vector($accessDao->query(['url' => $myUrl], ['sort' => ['on' => -1]]));

$attempts = <x:frag>
    <p>{count($results) . " accesses to $myUrl"}</p>
    <ui:accesses caption="Last Ten Here" results={$results->slice(0, 10)} />
</x:frag>;

$xhp = <layout:main title="MongoDB Crash Test">
    <h1>Crash Test</h1>
    <dl>
        <dt>User</dt>
        <dd>{$user->getEmail()}</dd>
        <dt>Password Hash</dt>
        <dd>{$user->getPasswordHash()}</dd>
    </dl>
    {$attempts}
    <table style="width:100%;">
        <tbody>
            <tr>
                <td style="width:50%;"><output id="attempt-1"/></td>
                <td><output id="attempt-2"/></td>
            </tr>
            <tr>
                <td><output id="attempt-3"/></td>
                <td><output id="attempt-4"/></td>
            </tr>
        </tbody>
    </table>
    <script src="main.js"></script>
</layout:main>;

echo $xhp;
