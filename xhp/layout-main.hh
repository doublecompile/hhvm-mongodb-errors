<?hh // strict

class :layout:main extends :x:element
{
    children (pcdata | %flow)*;
    attribute
        Stringish title = '';

    protected function render(): XHPRoot
    {
        return <x:doctype>
            <html lang="en">
                <head>
                    <meta charset="UTF-8" />
                    <title>{$this->:title}</title>
                    <style>{'
                        table {
                            font-size: 12px;
                        }
                    '}</style>
                </head>
                <body>
                    {$this->getChildren()}
                </body>
            </html>
        </x:doctype>;
    }
}
