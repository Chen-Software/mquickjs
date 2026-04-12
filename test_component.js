import { eval as jsEval } from 'local:microquickjs/engine';

const result = jsEval('2 + 2');
if (result.tag === 'ok') {
    console.log('Test OK: ' + result.val);
} else {
    console.log('Test Error: ' + result.val);
}
